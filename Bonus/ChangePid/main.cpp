#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <iostream>

int main() {
	int pipefd1[2], pipefd2[2];
	pid_t pid1, pid2;

	if (pipe(pipefd1) == -1 || pipe(pipefd2)) {
		perror("Pipe");
		exit(errno);
	}
	
	pid1 = fork();
	if (pid1 < 0) {
		perror("Pid1");
		exit(errno);
	}
	if (pid1 == 0) {
		close(pipefd1[0]);
		close(pipefd2[1]);

		pid_t my_pid = getpid();
		write(pipefd1[1], &my_pid, sizeof(my_pid));
		
		pid_t its_pid;
		read(pipefd2[0], &its_pid, sizeof(its_pid));

		std::cout << "Child 1 recieved of pid child 2 " << its_pid << std::endl;
		
		close(pipefd1[1]);
		close(pipefd2[0]);
		return 0;
	}

	pid2 = fork();
	if (pid2 < 0) {
		perror("Pid2");
		exit(errno);
	}
	if (pid2 == 0) {
		close(pipefd1[1]);
		close(pipefd2[0]);

		pid_t my_pid = getpid();
		write(pipefd2[1], &my_pid, sizeof(my_pid));
		
		pid_t its_pid;
		read(pipefd1[0], &its_pid, sizeof(its_pid));

		std::cout << "Child 2 recieved of pid child 1 " << its_pid << std::endl;

		close(pipefd2[1]);
		close(pipefd1[0]);
		return 0;
	}
	
	close(pipefd1[0]);
	close(pipefd1[1]);
	close(pipefd2[0]);
	close(pipefd2[1]);

	waitpid(pid1, nullptr, 0);
	waitpid(pid2, nullptr, 0);
	
	std::cout << "Parent process 	finishing!\n";
	return 0;
}











