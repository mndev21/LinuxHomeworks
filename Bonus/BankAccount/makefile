all: debit-credit

debit-credit: main.o
	g++ main.o -o debit-credit

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean: main.o
	rm *.o debit-credit
