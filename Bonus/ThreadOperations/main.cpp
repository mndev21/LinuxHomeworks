#include <iostream>
#include <pthread.h>
#include <vector>
#include <errno.h>
#include <cstring>

struct Task {
	int a, b;
	std::string sym;
	int id;
};

std::vector<int> result;
pthread_mutex_t mtx;

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
	pthread_mutex_lock(&mtx);
	result[task->id] = res;
	pthread_mutex_unlock(&mtx);
	
	return nullptr;
}

int main() {
	int N;
	std::cin >> N;
	
	result.resize(N);

	std::vector<pthread_t> threads(N);
	std::vector<Task> tasks(N);
	pthread_mutex_init(&mtx, nullptr);	

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

	for (int i = 0; i < N; ++i) {
		std::cout << "Task " << i + 1 << result[i] << "\n";
	}
	
	pthread_mutex_destroy(&mtx);

	return 0;
}
