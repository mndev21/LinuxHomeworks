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
    off_t current = 0;
    off_t file_size = lseek(src_fd, 0, SEEK_END);
    off_t hole_count = 0, data_count = 0;

    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    lseek(src_fd, -file_size, SEEK_END);
    while (true) {
	off_t data_start = lseek(src_fd, current, SEEK_DATA);
        if (data_start == -1) {
            perror("lseek (SEEK_DATA)");
            return 1;
        }
	
        off_t hole_start = lseek(src_fd, data_start, SEEK_HOLE);
        if (hole_start == -1) {
	    perror("lseek (SEEK_HOLE)");
            return 1;       
    	}
	
        data_count += hole_start - data_start;
        if (hole_start < file_size) {
            off_t next_data = lseek(src_fd, hole_start, SEEK_DATA);
            if (next_data == -1) {
               perror("lseek (SEEK_DATA after SEEK_HOLE)");
               return 1;
            }
       	    hole_count += next_data - hole_start;
	    total_bytes += hole_count + data_count;
       	    current = next_data;
	    bytes_read = read(src_fd, buffer, total_bytes);
	    write(dst_fd, buffer, total_bytes);
   	 }
	else {
	   break;
	}

    }
	
    close(src_fd);
    close(dst_fd);

    std::cout << "Successfully copied " << total_bytes << " bytes (data: " << (long long)data_count << ", hole: " << (long long)hole_count << ").\n";

    return 0;
}
