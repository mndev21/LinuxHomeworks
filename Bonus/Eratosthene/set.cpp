#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
#include <unistd.h>

int main() {
    int shm_fd = shm_open("array", O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm open");
        exit(errno);
    }

    int* data = static_cast<int*>(mmap(nullptr, 1000 * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    if (data == MAP_FAILED) {
        perror("mmap!");
        exit(errno);
    }

    for (int i = 0; i <= 1000; i++) {
        data[i] = 1;
    }

    data[0] = data[1] = 0;
    for (int p = 2; p * p <= 1000; p++) {
        if (data[p]) {
            for (int i = p * p; i <= 1000; i += p) {
                data[i] = 0;
            }
        }
    }

    close(shm_fd);

    return 0;
}