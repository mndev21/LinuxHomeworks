#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 3) {
        return errno;
    }

    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd == -1) {
        perror("open source file");
        return 1;
    }

    int dst_fd = open(argv[2], O_WRONLY);
    if (dst_fd == -1) {
        perror("open destination file");
        close(src_fd);
        return 1;
    }

    off_t current = 0;
    off_t file_size = lseek(src_fd, 0, SEEK_END);
    off_t holes = 0, data = 0;
    char buffer[BUFFER_SIZE];

    while (current < file_size) {
        off_t data_start = lseek(src_fd, current, SEEK_DATA);
        if (data_start == -1) {
            return errno;
        }

        if (data_start > current) {
            off_t hole_size = data_start - current;
	    holes += hole_size;
            lseek(dst_fd, hole_size, SEEK_CUR);
        }

        off_t hole_start = lseek(src_fd, data_start, SEEK_HOLE);
        if (hole_start == -1) {
            return errno;
        }

        off_t bytes_to_copy = hole_start - data_start;
	data += bytes_to_copy;
        lseek(src_fd, data_start, SEEK_SET);

        while (bytes_to_copy > 0) {
            ssize_t bytes_read = read(src_fd, buffer, (bytes_to_copy > BUFFER_SIZE) ? BUFFER_SIZE : bytes_to_copy);
            if (bytes_read == -1) {
                perror("read");
                close(src_fd);
                close(dst_fd);
                return errno;
            }

            ssize_t bytes_written = write(dst_fd, buffer, bytes_read);
            if (bytes_written == -1) {
                perror("write");
                close(src_fd);
                close(dst_fd);
                return errno;
            }

            bytes_to_copy -= bytes_read;
        }

        current = hole_start;
    }

    printf("File copied successfully with holes.\n");
    printf("Data_bytes: %lld\n", (long long)data);
    printf("Hole_bytes: %lld\n", (long long)holes);

    close(src_fd);
    close(dst_fd);
    return 0;
}

