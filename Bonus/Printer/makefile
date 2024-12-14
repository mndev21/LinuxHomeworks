all: printer

printer: main.o
	g++ main.o -o printer

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm *.o printer
