all: init destroy set check

init: init.o
	g++ -o init init.o

destroy: destroy.o
	g++ -o destroy destroy.o

set: set.o
	g++ -o set set.o

check: check.o
	g++ -o check check.o

init.o: init.cpp
	g++ -c init.cpp -o init.o

destroy.o: destroy.cpp
	g++ -c destroy.cpp -o destroy.o

set.o: set.cpp
	g++ -c set.cpp -o set.o

check.o: check.cpp
	g++ -c check.cpp -o check.o

clean:
	rm -f init destroy set check *.o
