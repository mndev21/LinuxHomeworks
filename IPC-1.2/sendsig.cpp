#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <csignal>

int main(int argc, char* argv[]) {
	if (argc < 3) {
		std::cerr << "Wrong count of variables\n";
		exit(1);
	}

	int signal_number = std::atoi(argv[1]);
	pid_t pid = std::atoi(argv[2]);

	if (kill(pid, signal_number) == -1) {
	        perror("Failed to send signal");
        	return 1;
    	}

	std::cout << "Signal " << signal_number << " sent to process " << pid << "\n";
    	return 0;
}
