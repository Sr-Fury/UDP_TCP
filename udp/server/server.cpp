#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8060); 
    server_addr.sin_addr.s_addr = INADDR_ANY; 

    bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    std::cout << "UDP working..." << std::endl;

    std::string user_input;

    char buffer[1024];
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_len);
        
        std::cout << "Client says: " << buffer << std::endl;

        while (true) {
            std::cout << "Write messege or exit: ";
            if (!std::getline(std::cin, user_input)) {
                std::cin.clear();
                continue;
            }
            
            if (!user_input.empty())
                break;
        }

        if (user_input == "exit")
            break;
        sendto(sockfd, user_input.c_str(), user_input.length(), 0, (struct sockaddr*)&client_addr, client_len);
    }

    std::cout << "Server closed:" << std::endl;
    close(sockfd);
    return 0;
}