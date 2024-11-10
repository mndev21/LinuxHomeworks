all: array-summary

array-summary: main.o
	g++ main.o -o array-summary

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm *.o array-summary
