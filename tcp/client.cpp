#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    std::cout << "Attempting to connect to the server..." << std::endl;

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cout << "Connection failed! Make sure the server is running." << std::endl;
        return -1;
    }

    std::cout << "Connected! You can start the conversation." << std::endl;

    std::string user_input;
    char buffer[1024];

    while (true) {
        while (true) {
            std::cout << "[You - Client]: ";
            if (!std::getline(std::cin, user_input)) {
                std::cin.clear();
                continue;
            }
            
            if (!user_input.empty())
                break;
        }

        if (user_input == "exit")
            break;

        send(sockfd, user_input.c_str(), user_input.length(), 0);

        std::cout << "(Waiting for server to reply...)" << std::endl;
        memset(buffer, 0, sizeof(buffer));
        
        int bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            std::cout << "Server closed the connection." << std::endl;
            break;
        }

        std::cout << "[Server]: " << buffer << std::endl;
    }

    close(sockfd);
    return 0;
}