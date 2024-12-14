#include <iostream>
#include <pthread.h>
#include <errno.h>

pthread_mutex_t mtx;
pthread_cond_t printer_cond;
bool is_available = true;

void* printer(void* arg) {
	const char* text = (const char*)arg;

	pthread_mutex_lock(&mtx);
	while (!is_available) {
		pthread_cond_wait(&printer_cond, &mtx);
	}

	is_available = false;
	std::cout << text << '\n';
	is_available = true;

	pthread_cond_signal(&printer_cond);
	pthread_mutex_unlock(&mtx);

	return nullptr;
}

int main() {
	pthread_mutex_init(&mtx, nullptr);
	pthread_cond_init(&printer_cond, nullptr);
	

	const char* texts[] = {"Message 1", "Message 2", "Message 3", "Message 4", "Message 5"};
    	pthread_t threads[5];

    	for (int i = 0; i < 5; ++i) {
        	if (pthread_create(&threads[i], nullptr, printer, (void*)texts[i]) != 0) {
            		perror("\n");
            		exit(errno);
        	}
    	}
    
    	for (int i = 0; i < 5; ++i) {
        	pthread_join(threads[i], nullptr);
    	}

    	pthread_mutex_destroy(&mtx);
   	pthread_cond_destroy(&printer_cond);
	return 0;
}
