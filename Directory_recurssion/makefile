all: travers-directory

travers-directory: main.o
	g++ main.o -o travers-directory

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm *.o travers-directory
