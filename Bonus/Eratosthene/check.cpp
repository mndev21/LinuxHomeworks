#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Wrong number of arguments\n";
        exit(0);
    }

    int N = atoi(argv[1]);

    int shm_fd = shm_open("array", O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Shm open");
        exit(errno);
    }

    int* data = static_cast<int*>(mmap(nullptr, 1000 * sizeof(int), PROT_READ, MAP_SHARED, shm_fd, 0));
    if (data == MAP_FAILED) {
        perror("mmap!");
        exit(errno);
    }

    if (data[N]) {
        std::cout << "Yes\n";
    }
    else {
        std::cout << "No\n";
    }
    close(shm_fd);
    return 0;
}