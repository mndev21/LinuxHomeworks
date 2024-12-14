#include <iostream>
#include <pthread.h>
#include <vector>
#include <errno.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

struct Task {
	int a, b;
	std::string sym;
	int id;
};

void* operation(void* arg) {
	Task* task = static_cast<Task*>(arg);
	int res = 0;
	int a = task->a, b = task->b;
	
	if (task->sym == "s") {
        	res = a + b;
    	} 
	else if (task->sym == "m") {
        	res = a * b;
    	} 
	else if (task->sym == "ss") {
        	res = a * a + b * b;
	}
	
	std::string filename = "out_" + std::to_string(task->id + 1);
	int fd = open(filename.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0) {
		perror("Open!");
		exit(errno);
	}
	
	std::string output = std::to_string(a) + ' ' + task->sym + ' ' + std::to_string(b) + " = " + std::to_string(res);

	ssize_t byte = write(fd, output.c_str(), output.size() + 1);
	if (byte < 0) {
		perror("Write!");
		exit(errno);
	}

	close(fd);
	
	return nullptr;
}

int main() {
	int N;
	std::cin >> N;
	
	std::vector<pthread_t> threads(N);
	std::vector<Task> tasks(N);	

	for (int i = 0; i < N; ++i) {
		std::cin >> tasks[i].a >> tasks[i].b >> tasks[i].sym;
		tasks[i].id = i;
		if (pthread_create(&threads[i], nullptr, operation, &tasks[i]) != 0) {
			perror("Pthread_create!");
			exit(errno);
		}
	}

	for (int i = 0; i < N; ++i) {
		pthread_join(threads[i], nullptr);
	}

	return 0;
}
