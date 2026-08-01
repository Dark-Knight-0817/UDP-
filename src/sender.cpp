#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <chrono>

#include "udp_packet.h"

int main()
{
    const char* SERVER_IP = "127.0.0.1"; 
    const int PORT = 9000;

    // 1. Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("socket");
        return 1;
    }

    // 2. Set up the server address
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    // 3. send data in a loop
    for(int i = 0; i < 20; ++i)
    {
        std::string msg = "packet #" + std::to_string(i);
        udp_packet packet;
        packet.seq = i;
        /**
         * @brief char *strncpy(char *dest, const char *src, size_t n)
         * @param dest--指向用于存储复制内容的目标数组
         *        src--指向要复制的源字符串
         *        n--要复制的最大字符数, C字符串后必须有'\0'结尾,
         */
        strncpy(packet.data, msg.c_str(), sizeof(packet.data) - 1);
        packet.data[sizeof(packet.data) - 1] = '\0';                // 确保以'\0'结尾
        // sendto(sockfd, msg.c_str(), msg.size() , 0, (sockaddr*)&server_addr, sizeof(server_addr));
        // std::cout << "Sent: " << msg << std::endl;
        sendto(sockfd, &packet, sizeof(packet), 0 , (sockaddr*)&server_addr, sizeof(server_addr));
        std::cout << "Sent packet seq: " << packet.seq << ", data: " << packet.data << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Done sending packets." << std::endl;
    close(sockfd);
    return 0;
}