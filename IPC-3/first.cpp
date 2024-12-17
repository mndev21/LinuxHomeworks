#include "shared_array.h"
#include <semaphore.h>

int main() {
    SharedArray array("sh_arr", 21);

    sem_t* write_sem = sem_open("write", O_CREAT, 0666, 1);
    sem_t* read_sem = sem_open("read", O_CREAT, 0666, 1);

    if (write_sem == SEM_FAILED) {
        perror("sem_open");
        exit(errno);
    }

    while (true) {
        sem_wait(write_sem);

        for (size_t i = 0; i < 21; ++i) {
            array[i] = i * 10;
        }
        std::cout << "First program execution!\n";

        sem_post(read_sem);
        sleep(2);
    }

    sem_close(write_sem);
    sem_close(read_sem);

    return 0;
}
