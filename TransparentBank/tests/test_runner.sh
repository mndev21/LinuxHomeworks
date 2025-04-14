#!/bin/bash

# Build the test executable
g++ -std=c++17 -Wall -Wextra -I../include test_bank.cpp ../src/BankAPI.cpp ../src/SharedMemory.cpp -o test_bank -lpthread

# Run the tests
./test_bank

# Run with valgrind for memory checking
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./test_bank

# Run with helgrind for thread checking
valgrind --tool=helgrind ./test_bank
