#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <thread>

void receive_messages(int socket_fd) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(socket_fd, buffer, sizeof(buffer), 0);
        
        if (bytes_received <= 0) {
            std::cout << "\nClient disconnected. Press Enter to exit." << std::endl;
            exit(0);
        }

        std::cout << "\n[Client]: " << buffer << "\n[You]: " << std::flush;
    }
}

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
    std::cout << "A client connected!\n[You]: " << std::flush;

    std::thread recv_thread(receive_messages, client_socket);
    recv_thread.detach();

    std::string server_reply;
    while (true) {
        if (!std::getline(std::cin, server_reply)) {
            std::cin.clear();
            continue;
        }
        
        if (server_reply.empty()) {
            std::cout << "[You]: " << std::flush;
            continue;
        }

        if (server_reply == "exit") {
            break;
        }

        send(client_socket, server_reply.c_str(), server_reply.length(), 0);
        std::cout << "[You]: " << std::flush;
    }

    close(client_socket);
    close(server_fd);
    return 0;
}