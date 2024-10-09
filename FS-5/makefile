all: manual-append

manual-append: manapp.o
	g++ manapp.o -o manual-append

manapp.o: manapp.cpp
	g++ -c manapp.cpp -o manapp.o

clean:
	rm *.o manual-append
