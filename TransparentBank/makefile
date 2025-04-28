CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = -pthread

SRC = src/BankAPI.cpp
OBJ = $(SRC:.cpp=.o)

all: BankInit BankDeinit BankClient

BankInit: $(OBJ) src/BankInit.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

BankDeinit: $(OBJ) src/BankDeinit.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

BankClient: $(OBJ) src/BankClient.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f BankInit BankDeinit BankClient *.o
