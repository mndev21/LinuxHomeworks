#ifndef PARALLEL_SCHEDULER_H
#define PARALLEL_SCHEDULER_H

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class parallel_scheduler {
private:
    std::vector<std::thread> threads;
    std::queue<std::pair<std::function<void(void*)>, void*>> task_queue;
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> stop;

    void worker_thread();

public:
    explicit parallel_scheduler(size_t capacity);
    ~parallel_scheduler();

    void run(std::function<void(void*)> func, void* arg);
};

#endif
