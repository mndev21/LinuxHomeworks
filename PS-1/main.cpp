#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <sys/wait.h>

void do_command(char** argv) {
	auto start = std::chrono::steady_clock::now();

	pid_t child = fork();
	if (child == -1) {
		exit(errno);
	}
	
	else if (child == 0) {
		execvp(argv[0], argv);
	}

	else {
		int status;
		if (waitpid(child, &status, 0) == -1) {
			exit(errno);
		}

		auto end = std::chrono::steady_clock::now();
        	std::chrono::duration<double> elapsed_seconds = end - start;

		if (WIFEXITED(status)) {
			std::cout << "Command completed with " << WEXITSTATUS(status) 
                      << " exit code and took " << elapsed_seconds.count() << " seconds.\n";
       		 } 
		else {
           		 std::cout << "Command did not terminate normally.\n";
		}
	}
}

int main(int argc, char** argv) {
	if (argc < 2) {
		return errno;
	}

	do_command(argv + 1);
	return 0;
}
