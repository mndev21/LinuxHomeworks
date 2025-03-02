#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>

int client_socket;

void* recieve_messages(void* arg) {
    char buffer[1024];

    while (true) {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received <= 0) {
		break;
	}
        buffer[bytes_received] = '\0';
        std::cout << buffer << "\n";
    }

    return nullptr;
}


int main(){

  // server address 
  struct sockaddr_in server_address;

  // create a socket
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
        perror("socket creation error");
        exit(errno);
  }

  // fill the server ip and port address 
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8888);

  // connect
  int connected = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));

  if(connected == -1){
    perror("connection failed");
    exit(errno);
  }
   
   std::string username;
   std::cout << "Enter your username: ";
   std::getline(std::cin, username);

   send(client_socket, username.c_str(), username.size(), 0);

   pthread_t recv_m;
   pthread_create(&recv_m, nullptr, recieve_messages, nullptr);
   pthread_detach(recv_m);

   // send message to server
  std::string mess;
  while (true) {
     std::getline(std::cin, mess);

     int sent = send(client_socket, mess.c_str(), mess.size(), 0);
     if(sent == -1){
       exit(errno);
     }
     if (mess == "/exit") {
     	break;
     }
  }

  close(client_socket);
  return 0;
}

