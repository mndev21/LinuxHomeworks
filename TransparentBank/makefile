CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS = -lpthread

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
BINARIES = $(BIN_DIR)/BankInit $(BIN_DIR)/BankDeinit $(BIN_DIR)/BankClient

.PHONY: all release debug test coverage clean

all: release

release: CXXFLAGS += -O3
release: $(BINARIES)

debug: CXXFLAGS += -g -O0 -DDEBUG
debug: $(BINARIES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/BankInit: $(OBJ_DIR)/BankInit.o $(OBJ_DIR)/BankAPI.o $(OBJ_DIR)/SharedMemory.o
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/BankDeinit: $(OBJ_DIR)/BankDeinit.o $(OBJ_DIR)/BankAPI.o $(OBJ_DIR)/SharedMemory.o
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/BankClient: $(OBJ_DIR)/BankClient.o $(OBJ_DIR)/BankAPI.o $(OBJ_DIR)/SharedMemory.o
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

test: debug
	@echo "Running tests..."
	@cd tests && ./test_runner.sh

coverage: CXXFLAGS += --coverage
coverage: LDFLAGS += --coverage
coverage: debug
	@echo "Running tests with coverage..."
	@cd tests && ./test_runner.sh
	@gcov -r $(OBJ_DIR)/*.gcno
	@lcov --capture --directory . --output-file coverage.info
	@genhtml coverage.info --output-directory coverage_report

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) coverage.info coverage_report *.gcov *.gcda *.gcno
