#include <iostream>
#include <pthread.h>
#include <vector>

class BankAccount {
private:
	int balance;
	pthread_mutex_t mtx;

public:
	BankAccount(): balance(0) {
		pthread_mutex_init(&mtx, nullptr);
	}

	BankAccount(int current_balance): balance(current_balance) {
		pthread_mutex_init(&mtx, nullptr);
	};

	~BankAccount() {
		pthread_mutex_destroy(&mtx);
	}

	void debit(int amount) {
		pthread_mutex_lock(&mtx);
		balance -= amount;
		pthread_mutex_unlock(&mtx);
	}

	void credit(int amount) {
		pthread_mutex_lock(&mtx);
		balance += amount;
		pthread_mutex_unlock(&mtx);
	}

	int getbalance() {
		return balance;
	}
};

struct ThreadArgs {
	BankAccount* account;
	int amount;
	bool isDebit;
};

void* performedebit(void *arg) {
	ThreadArgs* data = static_cast<ThreadArgs*>(arg);
	data->account->debit(data->amount);
	return nullptr;
}

void* performecredit(void* arg) {
	ThreadArgs* data = static_cast<ThreadArgs*>(arg);
	data->account->credit(data->amount);
	return nullptr;
}

int main() {
	BankAccount account(230);
	
	std::vector<pthread_t> threads(4);

	std::vector<ThreadArgs> threadData = {
        	{&account, 100, true},
        	{&account, 200, false},
        	{&account, 50, true},
        	{&account, 30, false},
    	};

	for (int i = 0; i < 4; ++i) {
		if (threadData[i].isDebit) {
			pthread_create(&threads[i], nullptr, performedebit, &threadData[i]);
		}
		else {
			pthread_create(&threads[i], nullptr, performecredit, &threadData[i]);
		}
	}

	for (int i = 0; i < 4; ++i) {
		pthread_join(threads[i], nullptr);
	}

	std::cout << "Final balance: " << account.getbalance() << std::endl;

	return 0;
}

















