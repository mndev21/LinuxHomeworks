#include "../include/SharedMemory.h"
#include <sys/shm.h>
#include <sys/ipc.h>
#include <cerrno>
#include <cstring>
#include <iostream>

static int shm_id = -1;

Account* SharedMemory::create(size_t num_accounts) {
    size_t size = num_accounts * sizeof(Account);
    
    shm_id = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
    if (shm_id == -1) {
        std::cerr << "Failed to create shared memory: " << strerror(errno) << std::endl;
        return nullptr;
    }
    
    void* ptr = shmat(shm_id, nullptr, 0);
    if (ptr == (void*)-1) {
        std::cerr << "Failed to attach shared memory: " << strerror(errno) << std::endl;
        return nullptr;
    }
    
    // Initialize accounts
    Account* accounts = new (ptr) Account[num_accounts];
    return accounts;
}

Account* SharedMemory::attach() {
    if (shm_id == -1) {
        std::cerr << "Shared memory not created yet" << std::endl;
        return nullptr;
    }
    
    void* ptr = shmat(shm_id, nullptr, 0);
    if (ptr == (void*)-1) {
        std::cerr << "Failed to attach shared memory: " << strerror(errno) << std::endl;
        return nullptr;
    }
    
    return static_cast<Account*>(ptr);
}

void SharedMemory::detach(Account* accounts) {
    if (accounts) {
        shmdt(accounts);
    }
}

void SharedMemory::destroy() {
    if (shm_id != -1) {
        shmctl(shm_id, IPC_RMID, nullptr);
        shm_id = -1;
    }
}
