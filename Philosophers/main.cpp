#include <iostream>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

const int N = 5;
sem_t* sticks[N];

void philosophy(int i) {
    int left = i;
    int right = (i + 1) % N;

    if (i == 0) {
        std::swap(left, right);
    }

    while (true) {
        std::cout << "Philosopher " << i + 1 << " is thinking...\n";
        sleep(3);

        sem_wait(sticks[left]);
        sem_wait(sticks[right]);

        std::cout << "Philosopher " << i + 1 << " is eating...\n";
        sleep(3);

        sem_post(sticks[left]);
        sem_post(sticks[right]);
    }
}

int main() {
    for (int i = 0; i < N; i++) {
        std::string name = "/stick" + std::to_string(i);
        sem_unlink(name.c_str());
        sticks[i] = sem_open(name.c_str(), O_CREAT, 0666, 1);
        if (sticks[i] == SEM_FAILED) {
            perror("sem_open");
            exit(1);
        }
    }

    for (int i = 0; i < N; ++i) {
        if (fork() == 0) {
            philosophy(i);
            exit(0);
        }
    }

    for (int i = 0; i < N; ++i) {
        wait(NULL);
    }

    for (int i = 0; i < N; ++i) {
        std::string name = "/stick" + std::to_string(i);
        sem_unlink(name.c_str());
    }

    return 0;
}

