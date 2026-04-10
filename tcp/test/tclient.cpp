#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
            std::cout << "\nServer closed the connection. Press Enter to exit." << std::endl;
            exit(0);
        }

        std::cout << "\n[Server]: " << buffer << "\n[You]: " << std::flush;
    }
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    std::cout << "Attempting to connect to the server..." << std::endl;

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cout << "Connection failed!" << std::endl;
        return -1;
    }

    std::cout << "Connected!\n[You]: " << std::flush;

    std::thread recv_thread(receive_messages, sockfd);
    recv_thread.detach();

    std::string user_input;
    while (true) {
        if (!std::getline(std::cin, user_input)) {
            std::cin.clear();
            continue;
        }
        
        if (user_input.empty()) {
            std::cout << "[You]: " << std::flush;
            continue;
        }

        if (user_input == "exit") {
            break;
        }

        send(sockfd, user_input.c_str(), user_input.length(), 0);
        std::cout << "[You]: " << std::flush;
    }

    close(sockfd);
    return 0;
}