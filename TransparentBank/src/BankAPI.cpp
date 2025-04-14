#include "../include/BankAPI.h"
#include "../include/SharedMemory.h"
#include <stdexcept>
#include <iostream>

Account* BankAPI::accounts = nullptr;
int BankAPI::num_accounts = 0;

bool BankAPI::initialize(int n) {
    if (n <= 0) return false;
    
    accounts = SharedMemory::create(n);
    if (!accounts) return false;
    
    num_accounts = n;
    return true;
}

void BankAPI::cleanup() {
    SharedMemory::detach(accounts);
    accounts = nullptr;
    num_accounts = 0;
}

bool BankAPI::validate_account(int account_id) {
    return account_id >= 0 && account_id < num_accounts;
}

bool BankAPI::get_balance(int account_id, int& balance) {
    if (!validate_account(account_id)) return false;
    
    std::lock_guard<std::mutex> lock(accounts[account_id].mtx);
    balance = accounts[account_id].current_balance;
    return true;
}

bool BankAPI::get_min_balance(int account_id, int& min_balance) {
    if (!validate_account(account_id)) return false;
    
    std::lock_guard<std::mutex> lock(accounts[account_id].mtx);
    min_balance = accounts[account_id].min_balance;
    return true;
}

bool BankAPI::get_max_balance(int account_id, int& max_balance) {
    if (!validate_account(account_id)) return false;
    
    std::lock_guard<std::mutex> lock(accounts[account_id].mtx);
    max_balance = accounts[account_id].max_balance;
    return true;
}

bool BankAPI::freeze_account(int account_id, bool freeze) {
    if (!validate_account(account_id)) return false;
    
    std::lock_guard<std::mutex> lock(accounts[account_id].mtx);
    accounts[account_id].is_frozen = freeze;
    return true;
}

bool BankAPI::transfer(int from_account, int to_account, int amount) {
    if (amount <= 0) return false;
    if (!validate_account(from_account) || !validate_account(to_account)) return false;
    if (from_account == to_account) return false;
    
    // Lock both accounts in a deadlock-free order
    if (from_account < to_account) {
        accounts[from_account].mtx.lock();
        accounts[to_account].mtx.lock();
    } else {
        accounts[to_account].mtx.lock();
        accounts[from_account].mtx.lock();
    }
    
    // Check if accounts are frozen
    if (accounts[from_account].is_frozen || accounts[to_account].is_frozen) {
        accounts[from_account].mtx.unlock();
        accounts[to_account].mtx.unlock();
        return false;
    }
    
    // Check if transfer is possible
    if (accounts[from_account].current_balance - amount < accounts[from_account].min_balance ||
        accounts[to_account].current_balance + amount > accounts[to_account].max_balance) {
        accounts[from_account].mtx.unlock();
        accounts[to_account].mtx.unlock();
        return false;
    }
    
    // Perform transfer
    accounts[from_account].current_balance -= amount;
    accounts[to_account].current_balance += amount;
    
    accounts[from_account].mtx.unlock();
    accounts[to_account].mtx.unlock();
    
    return true;
}

bool BankAPI::credit_all(int amount) {
    for (int i = 0; i < num_accounts; ++i) {
        std::lock_guard<std::mutex> lock(accounts[i].mtx);
        if (!accounts[i].is_frozen) {
            if (accounts[i].current_balance + amount > accounts[i].max_balance) {
                return false;
            }
        }
    }
    
    for (int i = 0; i < num_accounts; ++i) {
        std::lock_guard<std::mutex> lock(accounts[i].mtx);
        if (!accounts[i].is_frozen) {
            accounts[i].current_balance += amount;
        }
    }
    
    return true;
}

bool BankAPI::debit_all(int amount) {
    for (int i = 0; i < num_accounts; ++i) {
        std::lock_guard<std::mutex> lock(accounts[i].mtx);
        if (!accounts[i].is_frozen) {
            if (accounts[i].current_balance - amount < accounts[i].min_balance) {
                return false;
            }
        }
    }
    
    for (int i = 0; i < num_accounts; ++i) {
        std::lock_guard<std::mutex> lock(accounts[i].mtx);
        if (!accounts[i].is_frozen) {
            accounts[i].current_balance -= amount;
        }
    }
    
    return true;
}

bool BankAPI::set_min_balance(int account_id, int min_balance) {
    if (!validate_account(account_id)) return false;
    
    std::lock_guard<std::mutex> lock(accounts[account_id].mtx);
    if (min_balance > accounts[account_id].max_balance) return false;
    if (accounts[account_id].current_balance < min_balance) return false;
    
    accounts[account_id].min_balance = min_balance;
    return true;
}

bool BankAPI::set_max_balance(int account_id, int max_balance) {
    if (!validate_account(account_id)) return false;
    
    std::lock_guard<std::mutex> lock(accounts[account_id].mtx);
    if (max_balance < accounts[account_id].min_balance) return false;
    if (accounts[account_id].current_balance > max_balance) return false;
    
    accounts[account_id].max_balance = max_balance;
    return true;
}
