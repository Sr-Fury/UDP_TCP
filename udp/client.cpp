#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>

int main()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8060);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

    std::string user_input;

    char buffer[1024];
    socklen_t server_len = sizeof(server_addr);

    while (true)
    {
        std::cout << "Write messege or exit: ";
        std::getline(std::cin, user_input);

        if (user_input.empty())
            continue; 

        if (user_input == "exit")
            break;

        sendto(sockfd, user_input.c_str(), user_input.length(), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
        memset(buffer, 0, sizeof(buffer));
        
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, &server_len);
        
        std::cout << "Server says: " << buffer << std::endl;
    }

    std::cout << "Client closed:" << std::endl;
    close(sockfd);
    return 0;
}