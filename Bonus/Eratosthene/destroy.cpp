#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>

int main() {
    int shm_fd = shm_open("array", O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm open!");
        exit(errno);
    }

    void* shm_ptr = mmap(nullptr, 1000 * sizeof(int), PROT_READ, MAP_SHARED, shm_fd, 0);

    munmap(shm_ptr, 1000 * sizeof (int));
    close(shm_fd);
    shm_unlink("array");
    return 0;
}