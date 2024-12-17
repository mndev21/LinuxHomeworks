#include "shared_array.h"
#include <semaphore.h>

int main() {
    SharedArray array("sh_arr", 21);

    sem_t* write_sem = sem_open("write", 0);
    sem_t* read_sem = sem_open("read", 0);

    if (write_sem == SEM_FAILED) {
        perror("sem_open");
        exit(errno);
    }

    while (true) {
        sem_wait(read_sem);

        std::cout << "Second program: Array values:";
        for (size_t i = 0; i < 21; ++i) {
            std::cout << " " << array[i];
        }
        std::cout << std::endl;

        sem_post(write_sem);
        sleep(2);
    }

    sem_close(write_sem);
    sem_close(read_sem);

    return 0;
}
