#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <sys/shm.h>
#include <sys/ipc.h>
#include <cstddef>
#include "Account.h"

class SharedMemory {
public:
    static Account* create(size_t num_accounts);
    static Account* attach();
    static void detach(Account* accounts);
    static void destroy();
};

#endif // SHARED_MEMORY_H
