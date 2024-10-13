all: start

start: prog.o
	g++ prog.o -o start

prog.o: prog.cpp
	g++ -c prog.cpp -o prog.o

clean:
	rm *.o start
