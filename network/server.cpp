#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

pthread_mutex_t cout_mutex = PTHREAD_MUTEX_INITIALIZER;

void* handle_client(void* arg) {
    int client_socket = (int)(intptr_t)arg;
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

        pthread_mutex_lock(&cout_mutex);
        std::cout << "Received: " << buffer << "\n";
        pthread_mutex_unlock(&cout_mutex);

    }

    close(client_socket);
    return nullptr;
}

int main() {
    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Bind the socket to an address and port
    struct sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8888);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        close(server_socket);
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        close(server_socket);
        return 1;
    }

    std::cout << "Server is listening on port 8888...\n";

    while (true) {
        struct sockaddr_in client_address{};
        socklen_t client_addr_len = sizeof(client_address);

        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_addr_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        pthread_mutex_lock(&cout_mutex);
        std::cout << "New client connected: " << inet_ntoa(client_address.sin_addr) << "\n";
        pthread_mutex_unlock(&cout_mutex);

        pthread_t thread;
        if (pthread_create(&thread, nullptr, handle_client, (void*)(intptr_t)client_socket) != 0) {
            perror("Thread creation failed");
            close(client_socket);
        }
    }

    close(server_socket);
    pthread_mutex_destroy(&cout_mutex);
    return 0;
}