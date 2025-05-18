CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = -pthread

SRC = src/BankAPI.cpp
OBJ = $(SRC:.cpp=.o)

COLORPRINT_SRC = ./colorprint/colorprint.cpp
COLORPRINT_OBJ = $(COLORPRINT_SRC:.cpp=.o)
COLORPRINT_INC = -I./colorprint

all: BankInit BankDeinit BankClient BankServer BankRemoteClient

BankInit: $(OBJ) src/BankInit.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

BankDeinit: $(OBJ) src/BankDeinit.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

BankClient: $(OBJ) src/BankClient.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

BankServer: $(OBJ) src/BankServer.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

BankRemoteClient: $(OBJ) $(COLORPRINT_OBJ) src/BankRemoteClient.cpp
	$(CXX) $(CXXFLAGS) $(COLORPRINT_INC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(COLORPRINT_INC) -c $< -o $@

clean:
	rm -f BankInit BankDeinit BankClient BankServer BankRemoteClient *.o colorprint/*.o
