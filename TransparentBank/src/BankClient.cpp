#include "../include/BankAPI.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

void print_help() {
    std::cout << "Available commands:\n"
              << "  balance <account_id> - Get current balance\n"
              << "  min_balance <account_id> - Get minimum balance\n"
              << "  max_balance <account_id> - Get maximum balance\n"
              << "  freeze <account_id> <1/0> - Freeze (1) or unfreeze (0) account\n"
              << "  transfer <from> <to> <amount> - Transfer money between accounts\n"
              << "  credit_all <amount> - Credit all accounts\n"
              << "  debit_all <amount> - Debit all accounts\n"
              << "  set_min <account_id> <amount> - Set minimum balance\n"
              << "  set_max <account_id> <amount> - Set maximum balance\n"
              << "  help - Show this help\n"
              << "  exit - Exit the program\n";
}

bool process_command(const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;
    
    if (cmd == "exit") {
        return false;
    } else if (cmd == "help") {
        print_help();
    } else if (cmd == "balance") {
        int account_id;
        if (iss >> account_id) {
            int balance;
            if (BankAPI::get_balance(account_id, balance)) {
                std::cout << "Account " << account_id << " balance: " << balance << std::endl;
            } else {
                std::cout << "Failed to get balance for account " << account_id << std::endl;
            }
        } else {
            std::cout << "Invalid arguments for balance command" << std::endl;
        }
    } else if (cmd == "min_balance") {
        int account_id;
        if (iss >> account_id) {
            int min_balance;
            if (BankAPI::get_min_balance(account_id, min_balance)) {
                std::cout << "Account " << account_id << " min balance: " << min_balance << std::endl;
            } else {
                std::cout << "Failed to get min balance for account " << account_id << std::endl;
            }
        } else {
            std::cout << "Invalid arguments for min_balance command" << std::endl;
        }
    } else if (cmd == "max_balance") {
        int account_id;
        if (iss >> account_id) {
            int max_balance;
            if (BankAPI::get_max_balance(account_id, max_balance)) {
                std::cout << "Account " << account_id << " max balance: " << max_balance << std::endl;
            } else {
                std::cout << "Failed to get max balance for account " << account_id << std::endl;
            }
        } else {
            std::cout << "Invalid arguments for max_balance command" << std::endl;
        }
    } else if (cmd == "freeze") {
        int account_id, freeze;
        if (iss >> account_id >> freeze) {
            if (BankAPI::freeze_account(account_id, freeze != 0)) {
                std::cout << "Account " << account_id << " is now " 
                          << (freeze ? "frozen" : "unfrozen") << std::endl;
            } else {
                std::cout << "Failed to freeze/unfreeze account " << account_id << std::endl;
            }
        } else {
            std::cout << "Invalid arguments for freeze command" << std::endl;
        }
    } else if (cmd == "transfer") {
        int from, to, amount;
        if (iss >> from >> to >> amount) {
            if (BankAPI::transfer(from, to, amount)) {
                std::cout << "Transferred " << amount << " from account " << from 
                          << " to account " << to << std::endl;
            } else {
                std::cout << "Failed to transfer " << amount << " from account " << from 
                          << " to account " << to << std::endl;
            }
        } else {
            std::cout << "Invalid arguments for transfer command" << std::endl;
        }
    } else if (cmd == "credit_all") {
        int amount;
        if (iss >> amount) {
            if (BankAPI::credit_all(amount)) {
                std::cout << "Credited " << amount << " to all accounts" << std::endl;
            } else {
                std::cout << "Failed to credit " << amount << " to all accounts" << std::endl;
            }
        } else {
            std::cout << "Invalid arguments for credit_all command" << std::endl;
        }
    } else if (cmd == "debit_all") {
        int amount;
        if (iss >> amount) {
            if (BankAPI::debit_all(amount)) {
                std::cout << "Debited " << amount << " from all accounts" << std::endl;
            } else {
                std::cout << "Failed to debit " << amount << " from all accounts" << std::endl;
            }
        } else {
            std::cout << "Invalid arguments for debit_all command" << std::endl;
        }
    } else if (cmd == "set_min") {
        int account_id, amount;
        if (iss >> account_id >> amount) {
            if (BankAPI::set_min_balance(account_id, amount)) {
                std::cout << "Set min balance for account " << account_id 
                          << " to " << amount << std::endl;
            } else {
                std::cout << "Failed to set min balance for account " << account_id << std::endl;
            }
        } else {
            std::cout << "Invalid arguments for set_min command" << std::endl;
        }
    } else if (cmd == "set_max") {
        int account_id, amount;
        if (iss >> account_id >> amount) {
            if (BankAPI::set_max_balance(account_id, amount)) {
                std::cout << "Set max balance for account " << account_id 
                          << " to " << amount << std::endl;
            } else {
                std::cout << "Failed to set max balance for account " << account_id << std::endl;
            }
        } else {
            std::cout << "Invalid arguments for set_max command" << std::endl;
        }
    } else {
        std::cout << "Unknown command: " << cmd << std::endl;
        print_help();
    }
    
    return true;
}

int main() {
    std::cout << "Bank Client - Type 'help' for list of commands" << std::endl;
    
    std::string command;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);
        
        if (!process_command(command)) {
            break;
        }
    }
    
    return 0;
}
