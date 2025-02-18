#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

sem_t *t_sem, *p_sem, *m_sem, *barman_sem;
pid_t smokers[3];
const char* items = "tpm";

void smoker_T() {
    while (1) {
        sem_wait(t_sem);
        printf("T\n");
        sem_post(barman_sem);
    }
}

void smoker_P() {
    while (1) {
        sem_wait(p_sem);
        printf("P\n");
        sem_post(barman_sem);
    }
}

void smoker_M() {
    while (1) {
        sem_wait(m_sem);
        printf("M\n");
        sem_post(barman_sem);
    }
}

void barman() {
    char c;
    while (scanf(" %c", &c) == 1) {
        if (c == 't') {
            sem_post(t_sem);
        } 
	else if (c == 'p') {
            sem_post(p_sem);
        } 
	else if (c == 'm') {
            sem_post(m_sem);
        }
        sem_wait(barman_sem);
    }
    for (int i = 0; i < 3; i++) {
        kill(smokers[i], SIGTERM);
    }
}

int main() {
    t_sem = sem_open("t_sem", O_CREAT, 0644, 0);
    p_sem = sem_open("p_sem", O_CREAT, 0644, 0);
    m_sem = sem_open("m_sem", O_CREAT, 0644, 0);
    barman_sem = sem_open("barman_sem", O_CREAT, 0644, 1);

    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            if (items[i] == 't') {
                smoker_T();
            } else if (items[i] == 'p') {
                smoker_P();
            } else if (items[i] == 'm') {
                smoker_M();
            }
            exit(0);
        } else {
            smokers[i] = pid;
        }
    }

    barman();

    for (int i = 0; i < 3; i++) {
        waitpid(smokers[i], NULL, 0);
    }

    sem_close(t_sem);
    sem_close(p_sem);
    sem_close(m_sem);
    sem_close(barman_sem);

    sem_unlink("t_sem");
    sem_unlink("p_sem");
    sem_unlink("m_sem");
    sem_unlink("barman_sem");

    return 0;
}
