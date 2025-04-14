#include "../include/BankAPI.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_accounts>" << std::endl;
        return 1;
    }
    
    int num_accounts = std::atoi(argv[1]);
    if (num_accounts <= 0) {
        std::cerr << "Number of accounts must be positive" << std::endl;
        return 1;
    }
    
    if (!BankAPI::initialize(num_accounts)) {
        std::cerr << "Failed to initialize bank" << std::endl;
        return 1;
    }
    
    std::cout << "Bank initialized with " << num_accounts << " accounts" << std::endl;
    return 0;
}
