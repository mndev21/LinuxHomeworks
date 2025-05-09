#include "../include/BankAPI.h"
#include <iostream>

int main() {
    attach_bank();
    deinit_bank();
    std::cout << "Bank destroyed\n";
    return 0;
}
