all: pipe-operator

pipe-operator: main.o
	g++ main.o -o pipe-operator

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm *.o pipe-operator
