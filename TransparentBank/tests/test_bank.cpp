#include "../include/BankAPI.h"
#include <cassert>
#include <iostream>

void test_bank_operations() {
    // Initialize bank with 2 accounts
    assert(BankAPI::initialize(2));
    
    // Test initial balances
    int balance;
    assert(BankAPI::get_balance(0, balance));
    assert(balance == 0);
    assert(BankAPI::get_balance(1, balance));
    assert(balance == 0);
    
    // Test transfer
    assert(BankAPI::credit_all(100)); // Give both accounts 100
    assert(BankAPI::transfer(0, 1, 50));
    assert(BankAPI::get_balance(0, balance));
    assert(balance == 50);
    assert(BankAPI::get_balance(1, balance));
    assert(balance == 150);
    
    // Test freeze
    assert(BankAPI::freeze_account(0, true));
    assert(!BankAPI::transfer(0, 1, 10)); // Should fail
    assert(BankAPI::freeze_account(0, false));
    
    // Test limits
    assert(BankAPI::set_min_balance(0, -100));
    assert(BankAPI::set_max_balance(1, 200));
    
    // Cleanup
    BankAPI::cleanup();
    
    std::cout << "All tests passed!" << std::endl;
}

int main() {
    test_bank_operations();
    return 0;
}
