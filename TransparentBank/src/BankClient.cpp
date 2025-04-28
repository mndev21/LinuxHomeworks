#include "../include/BankAPI.h"
#include <iostream>

int main() {
    if (!attach_bank()) {
        std::cerr << "Failed to attach bank" << std::endl;
        return 1;
    }

    std::cout << "Bank client ready. Enter commands:\n";

    while (true) {
        std::cout << "> ";
        process_command();  // теперь процессит напрямую
    }

    detach_bank();
    return 0;
}
