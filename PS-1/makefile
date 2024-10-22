all: do-command

do-command: main.o
	g++ main.o -o do-command

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm *.o do-command
