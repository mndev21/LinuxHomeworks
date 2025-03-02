#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

int main() {
    const char *hostname = "httpforever.com";
    const char *request = "GET / HTTP/1.1\r\nHost: httpforever.com\r\nConnection: close\r\n\r\n";
    char buffer[100];

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = inet_addr("146.190.62.39");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(sock);
        return 1;
    }

    if (send(sock, request, strlen(request), 0) == -1) {
        perror("Send failed");
        close(sock);
        return 1;
    }

    int file_fd = open("httpforever.html", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_fd == -1) {
        perror("file open failed");
        close(sock);
        return 1;
    }

    ssize_t bytes_received;
    while ((bytes_received = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        write(file_fd, buffer, bytes_received);
    }

    if (bytes_received == -1) {
        perror("recv failed");
    }

    printf("Response saved to %s\n", "httpforever.html");

    close(file_fd);
    close(sock);
    return 0;
}
