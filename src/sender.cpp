#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <chrono>

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
        sendto(sockfd, msg.c_str(), msg.size() , 0, (sockaddr*)&server_addr, sizeof(server_addr));
        std::cout << "Sent: " << msg << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Done sending packets." << std::endl;
    close(sockfd);
    return 0;
}