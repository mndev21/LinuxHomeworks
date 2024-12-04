#include <iostream>
#include <csignal>
#include <unistd.h>
#include <atomic>

std::atomic<int> sigusr2_count(0);

void handle_sigusr1(int) {}

void handle_sigusr2(int) {
    ++sigusr2_count;
}

void handle_sigint(int) {
    std::cout << "\nReceived SIGUSR2 count: " << sigusr2_count << "\n";
    exit(0);
}

int main() {
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGUSR2, handle_sigusr2);
    signal(SIGINT, handle_sigint);

    std::cout << "Process PID: " << getpid() << std::endl;

    while (true) {
        std::cout << "I am still alive!!\n";
        sleep(5);
    }

    return 0;
}

