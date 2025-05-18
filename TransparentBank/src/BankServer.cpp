#include "../include/BankAPI.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <vector>

constexpr int PORT = 8888;
constexpr int MAX_CLIENTS = 10;

pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t cout_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

int request_count = 0;
bool server_running = true;

void* client_handler(void* arg) {
    int client_sock = (int)(intptr_t)arg;
    char buffer[1024];

    while (true) {
        ssize_t bytes_read = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read <= 0) {
            pthread_mutex_lock(&cout_mutex);
            std::cout << "Client disconnected\n";
            pthread_mutex_unlock(&cout_mutex);
            break;
        }

        buffer[bytes_read] = '\0';
        std::string command(buffer);

        if (command == "shutdown") {
            server_running = false;
            close(client_sock);
            break;
            //exit(0);
        }

        std::string recv;
        process_command(command, recv);

        pthread_mutex_lock(&clients_mutex);
        ssize_t bytes_sent = send(client_sock, recv.c_str(), recv.size(), 0);
        pthread_mutex_unlock(&clients_mutex);

        if (bytes_sent < 0) {
            perror("send");
            break;
        }

        // Увеличиваем счетчик и подаем сигнал
        pthread_mutex_lock(&counter_mutex);
        request_count++;
        if (request_count % 5 == 0) {
            pthread_cond_signal(&cond);
        }
        pthread_mutex_unlock(&counter_mutex);
    }

    close(client_sock);
    return nullptr;
}

void* stats_printer(void* arg) {
    (void)arg;
    while (server_running) {
        pthread_mutex_lock(&counter_mutex);
        pthread_cond_wait(&cond, &counter_mutex);
        std::cout << "Processed " << request_count << " requests\n";
        pthread_mutex_unlock(&counter_mutex);
    }
    return nullptr;
}

int main() {
    if (!attach_bank()) {
        std::cerr << "Failed to attach shared memory\n";
        return 1;
    }

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in server_addr {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_sock);
        return 1;
    }

    if (listen(server_sock, MAX_CLIENTS) < 0) {
        perror("listen");
        close(server_sock);
        return 1;
    }

    std::cout << "Server started on port " << PORT << "\n";

    pthread_t stats_thread;
    pthread_create(&stats_thread, nullptr, stats_printer, nullptr);

    while (server_running) {
        struct sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);

        int client_sock = accept(server_sock, (sockaddr*)&client_addr, &client_len);
        if (client_sock < 0) {
            perror("accept");
            continue;
        }

        pthread_t tid;
        if (pthread_create(&tid, nullptr, client_handler, (void*)(intptr_t)client_sock)) {
            perror("pthread_create");
            close(client_sock);
        }
        //pthread_detach(tid);
    }

    close(server_sock);
    detach_bank();
    return 0;
}
