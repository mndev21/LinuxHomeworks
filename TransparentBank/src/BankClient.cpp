#include "../include/BankAPI.h"
#include <iostream>

int main() {
    if (!attach_bank()) {
        std::cerr << "Failed to attach bank" << std::endl;
        return 1;
    }

    std::cout << "Bank client ready. Enter commands:\n";

    std::string command;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);
        if (command == "exit") break;
        process_command(command);
    }

    detach_bank();
    return 0;
}
