all: parallel_scheduler.so demo-application

parallel_scheduler.o: parallel_scheduler.cpp parallel_scheduler.h
	g++ -std=c++17 -Wall -pthread -fPIC -c parallel_scheduler.cpp

parallel_scheduler.so: parallel_scheduler.o
	g++ -shared -o parallel_scheduler.so parallel_scheduler.o

demo-application: demo.cpp parallel_scheduler.so
	g++ -std=c++17 -Wall -pthread -fPIC -o demo-application demo.cpp ./parallel_scheduler.so

clean:
	rm *.o *.so demo-application
