#include "../include/BankAPI.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <iostream>

static Bank* bank_ptr = nullptr;
static int shm_fd = -1;

bool init_bank(int n) {
    if (n > MAX_ACCOUNTS || n <= 0) return false;
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) return false;
    ftruncate(shm_fd, sizeof(Bank) + sizeof(Account) * n);
    bank_ptr = (Bank*)mmap(nullptr, sizeof(Bank) + sizeof(Account) * n, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (bank_ptr == MAP_FAILED) return false;
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&bank_ptr->mutex, &attr);
    bank_ptr->size = n;
    for (int i = 0; i < n; ++i) {
        bank_ptr->accounts[i] = {0, 0, 1000, false};
    }
    return true;
}

void deinit_bank() {
    if (bank_ptr) {
        pthread_mutex_destroy(&bank_ptr->mutex);
        munmap(bank_ptr, sizeof(Bank) + sizeof(Account) * bank_ptr->size);
        bank_ptr = nullptr;
    }
    if (shm_fd != -1) {
        close(shm_fd);
        shm_unlink(SHM_NAME);
    }
}

bool attach_bank() {
    shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) return false;
    bank_ptr = (Bank*)mmap(nullptr, sizeof(Bank) + sizeof(Account) * MAX_ACCOUNTS, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    return bank_ptr != MAP_FAILED;
}

void detach_bank() {
    if (bank_ptr) {
        munmap(bank_ptr, sizeof(Bank) + sizeof(Account) * MAX_ACCOUNTS);
        bank_ptr = nullptr;
    }
    if (shm_fd != -1) close(shm_fd);
}

void process_command() {
    if (!bank_ptr) {
        std::cout << "bank not initialized" << std::endl;
        return;
    }

    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    std::string op;
    iss >> op;

    pthread_mutex_lock(&bank_ptr->mutex);

    if (op == "get") {
        std::string type;
        int id;
        iss >> type >> id;
        if (id < 0 || id >= bank_ptr->size) std::cout << "invalid account" << std::endl;
        else {
            Account& acc = bank_ptr->accounts[id];
            if (type == "current") std::cout << acc.current << std::endl;
            else if (type == "min") std::cout << acc.min << std::endl;
            else if (type == "max") std::cout << acc.max << std::endl;
            else std::cout << "invalid field" << std::endl;
        }
    } else if (op == "freeze" || op == "thaw") {
        int id;
        iss >> id;
        if (id < 0 || id >= bank_ptr->size) std::cout << "invalid account" << std::endl;
        else {
            bank_ptr->accounts[id].frozen = (op == "freeze");
            std::cout << "done" << std::endl;
        }
    } else if (op == "transfer") {
        int from, to, x;
        iss >> from >> to >> x;
        if (from < 0 || from >= bank_ptr->size || to < 0 || to >= bank_ptr->size) {
            std::cout << "invalid account" << std::endl;
        } else if (x <= 0) {
            std::cout << "invalid amount" << std::endl;
        } else {
            Account& a = bank_ptr->accounts[from];
            Account& b = bank_ptr->accounts[to];
            if (a.frozen || b.frozen) std::cout << "account frozen" << std::endl;
            else if (a.current - x < a.min || b.current + x > b.max) std::cout << "limits exceeded" << std::endl;
            else {
                a.current -= x;
                b.current += x;
                std::cout << "done" << std::endl;
            }
        }
    } else if (op == "addall" || op == "suball") {
        int x;
        iss >> x;
        if (op == "suball") x = -x;
        bool ok = true;
        for (int i = 0; i < bank_ptr->size; ++i) {
            Account& acc = bank_ptr->accounts[i];
            if (acc.frozen || acc.current + x < acc.min || acc.current + x > acc.max) {
                ok = false;
                break;
            }
        }
        if (ok) {
            for (int i = 0; i < bank_ptr->size; ++i)
                bank_ptr->accounts[i].current += x;
            std::cout << "done" << std::endl;
        } else {
            std::cout << "operation failed" << std::endl;
        }
    } else if (op == "set") {
        std::string type;
        int id, x;
        iss >> type >> id >> x;
        if (id < 0 || id >= bank_ptr->size) std::cout << "invalid account" << std::endl;
        else {
            Account& acc = bank_ptr->accounts[id];
            if (type == "min") acc.min = x;
            else if (type == "max") acc.max = x;
            else {
                std::cout << "invalid type" << std::endl;
                pthread_mutex_unlock(&bank_ptr->mutex);
                return;
            }
            if (acc.current < acc.min || acc.current > acc.max) std::cout << "warning: current out of new limits" << std::endl;
            else std::cout << "done" << std::endl;
        }
    } else {
        std::cout << "unknown command" << std::endl;
    }

    pthread_mutex_unlock(&bank_ptr->mutex);
}
