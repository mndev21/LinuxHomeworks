#include "../include/BankAPI.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: init <num_accounts>\n";
        return 1;
    }
    int n = std::atoi(argv[1]);
    if (init_bank(n)) std::cout << "Bank initialized with " << n << " accounts\n";
    else std::cerr << "Initialization failed\n";
    return 0;
}
