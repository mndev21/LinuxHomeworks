#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <csignal>

int main() {
    pid_t parent_pid = getpid();
    std::cout << "Parent process started. PID: " << parent_pid << "\n";

    pid_t child_pid = fork();

    if (child_pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (child_pid == 0) {
        std::cout << "Child process started. PID: " << getpid() << "\n";
        sleep(2);

        std::cout << "Child process attempting to terminate parent (PID: " << parent_pid << ")...\n";

        if (kill(parent_pid, SIGKILL) == 0) {
            std::cout << "Parent process terminated successfully by child.\n";
        } else {
            perror("Failed to terminate parent process");
        }

        std::cout << "Child process exiting.\n";
        return 0;
    } 
    else {
        std::cout << "Parent process waiting for child...\n";
        int status;
        wait(&status);
        std::cout << "Parent process continues or exits based on child's actions.\n";

        if (WIFSIGNALED(status)) {
            std::cout << "Child process terminated with signal: " << WTERMSIG(status) << "\n";
        } else if (WIFEXITED(status)) {
            std::cout << "Child process exited with status: " << WEXITSTATUS(status) << "\n";
        }

        std::cout << "Parent process exiting.\n";
    }

    return 0;
}

