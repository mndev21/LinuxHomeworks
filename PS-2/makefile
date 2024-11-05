all: interactive-shell

interactive-shell: main.o
	g++ main.o -o interactive-shell

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm *.o interactive-shell
