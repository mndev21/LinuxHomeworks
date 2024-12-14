#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <errno.h>

void execute_command(const char* command, char* const args[]) {
    if (execvp(command, args) == -1) {
        perror("Execvp");
        exit(errno);
    }
}

void pipe_operator(char* const command1[], char* const command2[]) {
    int pipefd[2];
    pid_t pid1, pid2;

    if (pipe(pipefd) == -1) {
        perror("Pipe failed");
        exit(errno);
    }

    pid1 = fork();
    if (pid1 == -1) {
        perror("Fork failed for command1");
        exit(errno);
    }
    if (pid1 == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execute_command(command1[0], command1);
    }

    pid2 = fork();
    if (pid2 == -1) {
        perror("Fork failed for command2");
        exit(errno);
    }
    if (pid2 == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        execute_command(command2[0], command2);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);
}

int main() {
    char* const command1[] = { const_cast<char*>("ls"), nullptr };
    char* const command2[] = { const_cast<char*>("grep"), const_cast<char*>("cpp"), nullptr };

    pipe_operator(command1, command2);

    return 0;
}

