#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <mutex>

struct Account {
    int current_balance;
    int min_balance;
    int max_balance;
    bool is_frozen;
    std::mutex mtx;

    Account() : current_balance(0), min_balance(0), max_balance(10000), is_frozen(false) {}
};

#endif // ACCOUNT_H
