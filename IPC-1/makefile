all: signal-echo

signal-echo: main.o
	g++ main.o -o signal-echo

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm *.o signal-echo
