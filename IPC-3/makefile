all: first second

first: first.cpp
	g++ -o first first.cpp -lrt -lpthread

second: second.cpp
	g++ -o second second.cpp -lrt -lpthread

clean:
	rm -f first second
