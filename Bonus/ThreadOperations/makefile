all: operation

operation: main.o
	g++ main.o -o operation

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm *.o operation
