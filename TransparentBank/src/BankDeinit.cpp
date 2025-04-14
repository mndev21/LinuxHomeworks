#include "../include/BankAPI.h"
#include <iostream>

int main() {
    BankAPI::cleanup();
    std::cout << "Bank destroyed successfully" << std::endl;
    return 0;
}
