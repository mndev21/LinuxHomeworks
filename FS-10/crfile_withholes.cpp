#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

int main() {
    const char* file_path = "source_file.txt";
    
    // Open file for writing (create if not exists), truncate if exists
    int fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        std::cerr << "Error opening/creating file: " << strerror(errno) << std::endl;
        return 1;
    }

    // Write some data at the beginning
    const char* data1 = "DataBlock1";
    if (write(fd, data1, strlen(data1)) == -1) {
        std::cerr << "Error writing data: " << strerror(errno) << std::endl;
        close(fd);
        return 1;
    }

    // Create a hole by
    off_t hole_offset = 1024 * 1024;
    if (lseek(fd, hole_offset, SEEK_CUR) == -1) {
        std::cerr << "Error creating hole: " << strerror(errno) << std::endl;
        close(fd);
        return 1;
    }

    // Write more data after the hole
    const char* data2 = "DataBlock2";
    if (write(fd, data2, strlen(data2)) == -1) {
        std::cerr << "Error writing data: " << strerror(errno) << std::endl;
        close(fd);
        return 1;
    }

    // Create another hole
    hole_offset = 2 * 1024 * 1024;
    if (lseek(fd, hole_offset, SEEK_CUR) == -1) {
        std::cerr << "Error creating hole: " << strerror(errno) << std::endl;
        close(fd);
        return 1;
    }

    // Write more data after the second hole
    const char* data3 = "DataBlock3";
    if (write(fd, data3, strlen(data3)) == -1) {
        std::cerr << "Error writing data: " << strerror(errno) << std::endl;
        close(fd);
        return 1;
    }

    std::cout << "Sparse file with data and holes created successfully!" << std::endl;

    // Close the file
    close(fd);
    return 0;
}

