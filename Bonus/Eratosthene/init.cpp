#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>

int main() {
	int shm_fd = shm_open("array", O_CREAT | O_RDWR, 0666);
	if (shm_fd == -1) {
		perror("shm open");
        exit(errno);
	}

    if (ftruncate(shm_fd, 1000 * sizeof(int)) == -1) {
        perror("Set size!");
        exit(errno);
    }

    int* data = static_cast<int*>(mmap(nullptr, 1000 * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    if (data == MAP_FAILED) {
        perror("mmap!");
        exit(errno);
    }

    for (int i = 0; i < 1000; ++i) {
        data[i] = 0;
    }

    close(shm_fd);
    return 0;
}
