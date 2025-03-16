#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sstream>
#include <vector>
#include <queue>
#include <poll.h>

#define MAX_CLIENTS 100
#define THREAD_POOL_SIZE 10

pthread_mutex_t cout_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;

std::queue<int> client_queue;

double calculate(const std::string& op, double x, double y) {
    if (op == "ADD") return x + y;
    if (op == "SUB") return x - y;
    if (op == "MUL") return x * y;
    if (op == "DIV") return (y != 0) ? x / y : 0;
    return 0;
}

void* worker_thread(void*) {
    while (true) {
        pthread_mutex_lock(&queue_mutex);
        while (client_queue.empty()) {
            pthread_cond_wait(&queue_cond, &queue_mutex);
        }
        
        int client_socket = client_queue.front();
        client_queue.pop();
        pthread_mutex_unlock(&queue_mutex);

        char buffer[1024];
        while (true) {
            int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received <= 0) {
                pthread_mutex_lock(&cout_mutex);
                std::cout << "Client disconnected\n";
                pthread_mutex_unlock(&cout_mutex);
                break;
            }
            
            buffer[bytes_received] = '\0';
            std::string expression(buffer);
            std::stringstream ss(expression);
            
            std::string op;
            double x, y;
            ss >> op >> x >> y;
            double result = calculate(op, x, y);
            
            pthread_mutex_lock(&cout_mutex);
            std::cout << "Processed: " << result << "\n";
            pthread_mutex_unlock(&cout_mutex);
            
            std::string response = std::to_string(result) + "\n";
            send(client_socket, response.c_str(), response.size(), 0);
        }

        close(client_socket);
    }
    return nullptr;
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8888);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        close(server_socket);
        return 1;
    }

    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        close(server_socket);
        return 1;
    }

    std::cout << "Server listening on port 8888...\n";

    // Initialize thread pool
    pthread_t thread_pool[THREAD_POOL_SIZE];
    for (int i = 0; i < THREAD_POOL_SIZE; ++i) {
        pthread_create(&thread_pool[i], nullptr, worker_thread, nullptr);
    }

    struct pollfd fds[MAX_CLIENTS];
    fds[0].fd = server_socket;
    fds[0].events = POLLIN;
    int client_count = 1;

    while (true) {
        int poll_count = poll(fds, client_count, -1);
        if (poll_count < 0) {
            perror("Poll failed");
            break;
        }
        
        if (fds[0].revents & POLLIN) {
            struct sockaddr_in client_address;
            socklen_t client_addr_len = sizeof(client_address);
            int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_addr_len);
            if (client_socket >= 0) {
                pthread_mutex_lock(&cout_mutex);
                std::cout << "New client connected: " << inet_ntoa(client_address.sin_addr) << "\n";
                pthread_mutex_unlock(&cout_mutex);
                
                pthread_mutex_lock(&queue_mutex);
                client_queue.push(client_socket);
                pthread_cond_signal(&queue_cond);
                pthread_mutex_unlock(&queue_mutex);
            }
        }
    }

    close(server_socket);
    pthread_mutex_destroy(&cout_mutex);
    pthread_mutex_destroy(&queue_mutex);
    pthread_cond_destroy(&queue_cond);
    return 0;
}

