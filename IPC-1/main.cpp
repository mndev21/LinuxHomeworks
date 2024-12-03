#include <iostream>
#include <csignal>
#include <unistd.h>
#include <pwd.h>
#include <sys/ucontext.h> 

void sigusr1_handler(int signum, siginfo_t *info, void *context) {
    if (signum == SIGUSR1) {
        pid_t sender_pid = info->si_pid;
        uid_t sender_uid = info->si_uid;

        struct passwd *pwd = getpwuid(sender_uid);
        const char *username = pwd ? pwd->pw_name : "unknown";

        ucontext_t *ctx = (ucontext_t *)context;
	
	std::cout << "Received a SIGUSR1 signal from process " << sender_pid
                  << " executed by UID " << sender_uid << " (" << username << ").\n";
        #if defined(__x86_64__)
        auto rip = ctx->uc_mcontext.gregs[REG_RIP];
        auto rax = ctx->uc_mcontext.gregs[REG_RAX];
        auto rbx = ctx->uc_mcontext.gregs[REG_RBX];
        std::cout << "State of the context: RIP = " << std::hex << rip
                  << ", RAX = " << std::hex << rax
                  << ", RBX = " << std::hex << rbx << ".\n";
        #elif defined(__i386__)
        auto eip = ctx->uc_mcontext.gregs[REG_EIP];
        auto eax = ctx->uc_mcontext.gregs[REG_EAX];
        auto ebx = ctx->uc_mcontext.gregs[REG_EBX];
        std::cout << "State of the context: EIP = " << std::hex << eip
                  << ", EAX = " << std::hex << eax
                  << ", EBX = " << std::hex << ebx << ".\n";
        #else
        std::cerr << "Unsupported architecture for accessing registers.\n";
        #endif
    }
}

int main() {
    std::cout << "Process PID: " << getpid() << "\n";

    struct sigaction action{};
    action.sa_sigaction = sigusr1_handler;
    action.sa_flags = SA_SIGINFO;

    if (sigaction(SIGUSR1, &action, nullptr) < 0) {
        perror("sigaction");
        return 1;
    }

    while (true) {
        std::cout << "Waiting for SIGUSR1...\n";
        sleep(10);
    }

    return 0;
}

