all: ph

ph: main.o
	g++ main.o -o ph

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm *.o ph
