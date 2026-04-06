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
    server_addr.sin_port = htons(8080); 
    server_addr.sin_addr.s_addr = INADDR_ANY; 

    bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    std::cout << "UDP working..." << std::endl;

    char buffer[1024];
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_len);
        
        std::cout << "Received data: " << buffer << std::endl;

        const char* reply_msg = "I got your message!";
        sendto(sockfd, reply_msg, strlen(reply_msg), 0, (struct sockaddr*)&client_addr, client_len);
    }

    close(sockfd);
    return 0;
}