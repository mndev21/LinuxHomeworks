#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <sys/stat.h>

#define BUFFER_SIZE 4096

int main(int argc, char** argv) {
    if (argc != 3) {
	std::cout << errno;
        return 1;
    }

    const char* src_file = argv[1];
    const char* dst_file = argv[2];

    int src_fd = open(src_file, O_RDONLY);
    if (src_fd < 0) {
        return errno;
    }

    int dst_fd = open(dst_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd < 0) {
        close(src_fd);
        return errno;
    }

    off_t total_bytes = 0;
    off_t data_bytes = 0;
    off_t hole_bytes = 0;

    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        total_bytes += bytes_read;

        bool hole_detected = true;
        for (ssize_t i = 0; i < bytes_read; ++i) {
            if (buffer[i] != '\0') {
                hole_detected = false;
                break;
            }
        }

        if (hole_detected) {
            hole_bytes += bytes_read;

            lseek(dst_fd, bytes_read, SEEK_CUR);
        }
       	else {
            size_t bytes_written = write(dst_fd, buffer, bytes_read);
            if (bytes_written < 0) {
                perror("Error writing to destination file");
                close(src_fd);
                close(dst_fd);
                return errno;
            }
            data_bytes += bytes_written;
        }
    }

    if (bytes_read < 0) {
        perror("Error reading source file");
    }

    close(src_fd);
    close(dst_fd);

    std::cout << "Successfully copied " << total_bytes << " bytes (data: " << data_bytes << ", hole: " << hole_bytes << ").\n";

    return 0;
}
