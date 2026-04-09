#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <string>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_fd, 5);
    std::cout << "TCP Server is listening on port 8080..." << std::endl;

    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    std::cout << "A client connected! You can start talking." << std::endl;

    char buffer[1024];
    std::string server_reply;

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        
        if (bytes_received <= 0) {
            std::cout << "Client disconnected." << std::endl;
            break;
        }

        std::cout << "\n[Client]: " << buffer << std::endl;

        while (true) {
            std::cout << "[You - Server]: ";
            if (!std::getline(std::cin, server_reply)) {
                std::cin.clear();
                continue;
            }
            
            if (!server_reply.empty())
                break;
        }

        if (server_reply == "exit")
            break;

        send(client_socket, server_reply.c_str(), server_reply.length(), 0);
    }

    close(client_socket);
    close(server_fd);
    return 0;
}