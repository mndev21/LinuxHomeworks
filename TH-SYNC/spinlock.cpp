#include <iostream>
#include <pthread.h>
#include <chrono>
#include <unistd.h>
#include <fcntl.h>

pthread_spinlock_t spinlock;

class Integer {
private:
  int inner_value = 0;
public:
  Integer(int value) :inner_value(value) {}
  void inc() {
    pthread_spin_lock(&spinlock);
    inner_value++;
    pthread_spin_unlock(&spinlock);
  }
  int get_inner_value() {
    return inner_value;
  }
};

void* increment(void* arg) {
  Integer* num = (Integer*)arg;
  for (int i = 0; i < 10000; ++i) {
    num->inc();
  }
  return nullptr;
}

int main(int argc, char** argv) {
  Integer* num = new Integer(0);
  auto start = std::chrono::high_resolution_clock::now();
  pthread_spin_init(&spinlock, 0);
  int thread_count = 50;
  pthread_t* tids = new pthread_t[thread_count];

  for (int i = 0; i < thread_count; ++i) {
    pthread_create(&tids[i], NULL, increment, num);  
  }

  for (int i = 0; i < thread_count; ++i) {
    pthread_join(tids[i], NULL);
  }
  
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> duration = end - start;
  std::cout << "Final value is " << num->get_inner_value() << '\n';
  std::cout << "Spinlock time - " << duration.count() << '\n';
  
  int fd = open("timer", O_APPEND | O_WRONLY, 0644);
  if (fd < 0) {
  	perror("Something went wrong!");
	exit(1);
  }

  if (dup2(fd, 1) < 0) {
	perror("Something went wrong!");
        exit(1);
  }
  
  std::cout << "Spinlock time - " << duration.count() << '\n';

  if (close(fd) < 0) {
  	exit(0);
  }

    
  pthread_spin_destroy(&spinlock);
  delete[] tids;
  delete num;
  return 0;
}

