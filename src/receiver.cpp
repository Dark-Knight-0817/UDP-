#include <iostream>
#include <cstring>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    const int PORT = 9000;
    // 1. Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("socket");
        return 1;
    }

    // 2. Bind the socket to a specific port
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;  // 监听所有网卡
    addr.sin_port = htons(PORT);        // 将端口号转换为网络字节序
    if(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0){
        perror("bind");
        close(sockfd);
        return 1;
    }

    // 3. Receive data
    char buffer[1024];
    sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);
    while(true)
    {
        ssize_t recv_len = recvfrom( sockfd, buffer, sizeof(buffer) - 1, 0,
                                    (struct sockaddr*)&client_addr, &client_addr_len);
        if(recv_len < 0)
        {
            perror("recvfrom");
            continue;
        }
        buffer[recv_len] = '\0';  // Null-terminate the received data
        std::cout << "Received:" << buffer << std::endl;
    }

    close(sockfd);
    return 0;
}
