#include <iostream>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>

sem_t sticks[5];

void philosophy(int i) {
	int left = i;
        int right = (i + 1) % 5;

	if (i == 0) {
		int temp = left;
		left = right;
		right = temp;
	}

	while (1) {
		printf("Philosoph %d I am thinking...\n", i + 1);
		sleep(3);
		sem_wait(&sticks[left]);
		sem_wait(&sticks[right]);
		std::cout << "Philosoph " << i + 1 << " I am eating\n";
	        sleep(3);
		sem_post(&sticks[left]);
		sem_post(&sticks[right]);
	}

	exit(0);
}

int main() {
	for (int i = 0; i < 5; i++) {
		sem_init(&sticks[i], 1, 1);
	}

	for (int i = 0; i < 5; ++i) {
		if (fork() == 0) {
			philosophy(i);
			exit(0);
		}
	}

	for (int i = 0; i < 5; ++i) {
		wait(NULL);
	}

	for (int i = 0; i < 5; ++i) {
		sem_destroy(&sticks[i]);
	}

	return 0;
}
