#include <fcntl.h>
#include <cstdio>
#include <unistd.h>
#include <cerrno>

#define BUFFER_SIZE 4096

int main() {
    char src_name[100];
    printf("Enter the name of source file: ");
    scanf("%s", src_name);

    int src_fd = open(src_name, O_RDONLY);
    if (src_fd == -1) {
        perror("open source file");
        return 1;
    }

    char dst_name[100];
    printf("Enter the name of destination file: ");
    scanf("%s", dst_name);

    int dst_fd = open(dst_name, O_WRONLY);
    if (dst_fd == -1) {
        perror("open destination file");
        close(src_fd);
        return 1;
    }

    off_t src_offset, dst_offset;
    size_t byte_size;

    printf("Enter the initial offset in the source file (in bytes): ");
    scanf("%ld", &src_offset);

    printf("Enter the initial offset in the destination file (in bytes): ");
    scanf("%ld", &dst_offset);
    if (lseek(dst_fd, dst_offset, SEEK_SET) < 0) {
        perror("Failed to seek in destination file");
        close(src_fd);
        close(dst_fd);
        return errno;
    }


    printf("Enter the number of bytes to copy: ");
    scanf("%ld", &byte_size);


    off_t current = src_offset;
    off_t holes = 0, data = 0;
    char buffer[BUFFER_SIZE];

    while (current < byte_size) {
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

