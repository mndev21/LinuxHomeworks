#include "parallel_scheduler.h"

parallel_scheduler::parallel_scheduler(size_t capacity) : stop(false) {
    for (size_t i = 0; i < capacity; ++i) {
        threads.emplace_back(&parallel_scheduler::worker_thread, this);
    }
}

parallel_scheduler::~parallel_scheduler() {
    stop.store(true);
    condition.notify_all();
    for (std::thread& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void parallel_scheduler::run(std::function<void(void*)> func, void* arg) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        task_queue.emplace(func, arg);
    }
    condition.notify_one();
}

void parallel_scheduler::worker_thread() {
    while (!stop) {
        std::function<void(void*)> task;
        void* arg;

        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            condition.wait(lock, [this]() { return stop || !task_queue.empty(); });

            if (stop && task_queue.empty()) return;

            task = task_queue.front().first;
            arg = task_queue.front().second;
            task_queue.pop();
        }

        task(arg);
    }
}
