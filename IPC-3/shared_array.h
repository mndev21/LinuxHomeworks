#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <csignal>

class SharedArray {
private:
	int* data;
	const char* name;
	std::size_t size;
	int shm_fd;

public:
	SharedArray(const char* name_, std::size_t size_): name(name_), size(size_) {
		shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
		if (shm_fd == -1) {
			perror("Shm open!");
			exit(errno);
		}

		if (ftruncate(shm_fd, size * sizeof(int)) == -1) {
			perror("Set size!");
			exit(errno);
		}

		data = static_cast<int*>(mmap(nullptr, size * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
		if (data == MAP_FAILED) {
			perror("mmap");
			exit(errno);
		}

	}

	const int& operator[](std::size_t index) const {
		if (index < size) {
			return data[index];
		}
        return errno;
	}

	int& operator[](std::size_t index) {
		if (index < size) {
			return data[index];
		}
        return errno;
	}

	~SharedArray() {
		munmap(data, size * sizeof(int));
		close(shm_fd);
		shm_unlink(name);
	}

};
