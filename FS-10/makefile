all: copy

copy: copy.o
	g++ copy.o -o copy

copy.o: copy.cpp
	g++ -c copy.cpp -o copy.o

clean:
	rm *.o copy
