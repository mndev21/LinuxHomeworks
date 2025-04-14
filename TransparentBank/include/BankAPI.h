#ifndef BANK_API_H
#define BANK_API_H

#include "Account.h"
#include <string>

class BankAPI {
public:
    static bool initialize(int num_accounts);
    static void cleanup();
    
    static bool get_balance(int account_id, int& balance);
    static bool get_min_balance(int account_id, int& min_balance);
    static bool get_max_balance(int account_id, int& max_balance);
    
    static bool freeze_account(int account_id, bool freeze);
    
    static bool transfer(int from_account, int to_account, int amount);
    static bool credit_all(int amount);
    static bool debit_all(int amount);
    
    static bool set_min_balance(int account_id, int min_balance);
    static bool set_max_balance(int account_id, int max_balance);
    
private:
    static Account* accounts;
    static int num_accounts;
    
    static bool validate_account(int account_id);
};

#endif // BANK_API_H
