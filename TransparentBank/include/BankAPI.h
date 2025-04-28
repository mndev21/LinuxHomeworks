#pragma once
#include <pthread.h>
#include <string>

constexpr const char* SHM_NAME = "/transparent_bank";
constexpr int MAX_ACCOUNTS = 1024;

struct Account {
    int current = 0;
    int min = 0;
    int max = 1000;
    bool frozen = false;
};

struct Bank {
    pthread_mutex_t mutex;
    int size;
    Account accounts[];
};

bool init_bank(int n);
void deinit_bank();
bool attach_bank();
void detach_bank();
void process_command();
