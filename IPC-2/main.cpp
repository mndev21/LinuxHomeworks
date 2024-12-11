#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <string>
#include <cmath>
#include <cstdlib>

int prime_numberN(int n) {
    int count = 0;
    int num = 1;
    while (count != n) {
        ++num;
        bool isprime = true;
        for (int i = 2; i <= sqrt(num); ++i) {
            if (num % i == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            ++count;
        }
    }
    return num;
}

int main() {
    int fd1[2];
    int fd2[2];

    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("Pipe");
        exit(errno);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork");
        exit(errno);
    }

    else if (pid == 0) {
        close(fd1[1]);
        close(fd2[0]);

        while (true) {
            char buffer[100];
            int bytes_read = read(fd1[0], buffer, sizeof(buffer) - 1);
            if (bytes_read <= 0) {

                if (bytes_read < 0) {
                    perror("Read");
                }
                break;
            }

            buffer[bytes_read] = '\0';

            if (std::string(buffer) == "exit") {
                std::cout << "[Child] Exiting...\n";
                break;
            }

            std::cout << "[Child] Calculating " << buffer << "-th prime number...\n";
            int prime = prime_numberN(atoi(buffer));

            std::cout << "[Child] Sending calculation result of prime(" << buffer << ")...\n";
            if (write(fd2[1], &prime, sizeof(prime)) < 0) {
                perror("Write");
                break;
            }
        }

        close(fd1[0]);
        close(fd2[1]);
    }

    else {
        close(fd1[0]);
        close(fd2[1]);

        std::string input;
        while (true) {
            std::cout << "[Parent] Please enter the number: ";
            std::getline(std::cin, input);

            if (write(fd1[1], input.c_str(), input.size() + 1) < 0) {
                perror("Write");
                exit(errno);
            }

            if (input == "exit") {
                break;
            }

            std::cout << "[Parent] Sending " << input << " to the child process...\n";
            std::cout << "[Parent] Waiting for the response from the child process...\n";

            int prime;
            int bytes_read = read(fd2[0], &prime, sizeof(prime));
            if (bytes_read <= 0) {
                if (bytes_read < 0) {
                    perror("Read");
                }
                break;
            }

            std::cout << "[Parent] Received calculation result of prime(" << input << ") = " << prime << "...\n\n";
        }

        close(fd1[1]);
        close(fd2[0]);
    }

    return 0;
}
