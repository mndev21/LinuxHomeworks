#include "parallel_scheduler.h"
#include <iostream>
#include <chrono>

void example_task(void* arg) {
    int task_id = *static_cast<int*>(arg);
    std::cout << "Task " << task_id << " is running on thread " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Task " << task_id << " is completed." << std::endl;
}

int main() {
    constexpr size_t pool_size = 4;
    parallel_scheduler scheduler(pool_size);

    constexpr int num_tasks = 10;
    int task_ids[num_tasks];
    for (int i = 0; i < num_tasks; ++i) {
        task_ids[i] = i + 1;
        scheduler.run(example_task, &task_ids[i]);
    }

    std::this_thread::sleep_for(std::chrono::seconds(12));
    return 0;
}
