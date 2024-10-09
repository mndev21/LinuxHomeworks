#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>

void initialize(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input-file>" << std::endl;
        exit(1);
    }

    const char* filePath = argv[1];
    int fileDescriptor = open(filePath, O_RDONLY);
    if (fileDescriptor < 0) {
        perror("Error opening file");
        exit(1);
    }

    if (dup2(fileDescriptor, 0) < 0) {
        perror("Error redirecting stdin");
        close(fileDescriptor);
        exit(1);
    }

    close(fileDescriptor);
}

int main(int argc, char** argv) {
    initialize(argc, argv);
 
    std::string input;
    std::cin >> input;
    
    std::string reversed(input.rbegin(), input.rend());

    std::cout << reversed << std::endl;

    return 0;
}

