all: prime-calculator

prime-calculator: main.o
	g++ main.o -o prime-calculator

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm *.o prime-calculator
