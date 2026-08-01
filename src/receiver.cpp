#include <iostream>
#include <cstring>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

 #include "udp_packet.h"

/**
 * @note UDP数据报,每次都是1个报文
 *       一个完整的数据报长度由返回值决定.天然具有边界性
 *       所以,如果send > recv, recv 无法一次性接收,就会全部扔掉,而不会部分保存到缓冲区
 */
udp_packet analyse_packet(char *buffer, int len)
{
    udp_packet packet;
    // 1. 解析序号
    // &是取地址, *是解引用
    packet.seq = *(int32_t*)buffer; // 硬件架构决定buffer地址字节数,int32_t和char只是决定数据的解读方式
    // 2. 解析内容
    memcpy(packet.data, buffer + sizeof(int32_t), len - sizeof(int32_t));
    return packet;
}

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
        // 版本1: 直接打印接收到的内容
        // buffer[recv_len] = '\0';  // Null-terminate the received data
        // std::cout << "Received:" << buffer << std::endl;
        // 版本2: 使用固定结构体解析数据报
        udp_packet packet = *(udp_packet*)buffer; // 固定结构体时,可以用这种方法
        std::cout << "Received packet seq: " << packet.seq << ", data: " << packet.data << std::endl;

    }

    close(sockfd);
    return 0;
}
