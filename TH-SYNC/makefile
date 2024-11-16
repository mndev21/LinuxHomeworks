all: synchronization

synchronization: mutex.o semaphore.o spinlock.o
	g++ mutex.o semaphore.o spinlock.o -o synchronization

mutex.o: mutex.cpp
	g++ -c mutex.cpp -o mutex.o

semaphore.o: semaphore.cpp
	g++ -c semaphore.cpp -o semaphore.o

spinlock.o: spinlock.cpp
	g++ -c spinlock.cpp -o spinlock.o

clean:
	rm *.o synchronization
