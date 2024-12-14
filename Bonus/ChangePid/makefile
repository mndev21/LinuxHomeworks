all: change-pid

change-pid: main.o
	g++ main.o -o change-pid

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm *.o change-pid
