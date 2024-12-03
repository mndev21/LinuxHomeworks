all: kill-parent

kill-parent: main.o
	g++ main.o -o kill-parent

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm *.o kill-parent
