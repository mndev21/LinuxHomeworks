all: bar

bar: main.o
	g++ main.o -o bar

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm *.o bar
