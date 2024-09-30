#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file-to-erase>" << std::endl;
        return 1;
    }

    const char* filePath = argv[1];
    struct stat fileStat;

    if (stat(filePath, &fileStat) != 0) {
        std::cerr << "Error: Unable to access file: " << strerror(errno) << std::endl;
        return 1;
    }

    int fileDesc = open(filePath, O_WRONLY);
    if (fileDesc < 0) {
        std::cerr << "Error: Unable to open file: " << strerror(errno) << std::endl;
        return 1;
    }

    off_t fileSize = fileStat.st_size;
    char nullByte = '\0';
    for (off_t i = 0; i < fileSize; ++i) {
        if (write(fileDesc, &nullByte, 1) != 1) {
            std::cerr << "Error: Unable to overwrite file: " << strerror(errno) << std::endl;
            close(fileDesc);
            return 1;
        }
    }

    close(fileDesc);

    if (unlink(filePath) != 0) {
        std::cerr << "Error: Unable to delete file: " << strerror(errno) << std::endl;
        return 1;
    }

    std::cout << "File securely deleted." << std::endl;
    return 0;
}

