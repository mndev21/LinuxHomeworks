#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void executeCommand(const std::string &command, bool silent) {
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed." << std::endl;
        return;
    } else if (pid == 0) {
        std::string path = getenv("PATH");
        path = "./:" + path;
        setenv("PATH", path.c_str(), 1);

        std::string cmd = command.substr(silent ? 7 : 0);
        char *args[64];
        char *token = strtok(&cmd[0], " ");
        int i = 0;
        while (token != nullptr) {
            args[i++] = token;
            token = strtok(nullptr, " ");
        }
        args[i] = nullptr;

        if (silent) {
            std::string filename = std::to_string(getpid()) + ".log";
            int fd = open(filename.c_str(), O_WRONLY | O_CREAT, 0644);
            if (fd == -1) {
                std::cerr << "Failed to open log file." << std::endl;
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);
        }

        execvp(args[0], args);
        std::cerr << "Command execution failed." << std::endl;
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

int main() {
    std::string input;
    while (true) {
        std::getline(std::cin, input);

        if (input == "exit") break;
        if (input.empty()) continue;

        bool silent = input.find("silent") == 0;
        executeCommand(input, silent);
    }
    return 0;
}

