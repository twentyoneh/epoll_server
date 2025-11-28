#include "UdpSocket.h"

#include <cstring>
#include <cstdio>
#include <stdexcept>
#include <cerrno>

UdpSocket::UdpSocket(int port, int epoll_fd) {
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) throw std::runtime_error("socket udp failed");

    if (set_nonbloking(fd) == -1) {
        close(fd);
        throw std::runtime_error("Failed to set non-blocking mode for UDP socket.");
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    if (bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        throw std::runtime_error("bind udp failed");
    }

    epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        throw std::runtime_error("epoll_ctl: udp_fd failed");
    }
    std::cout << "UDP listening on port " << port << std::endl;
}

void UdpSocket::handle_data(struct sockaddr_in& client_addr, socklen_t& client_len, bool& shutdown_requested) {
        char buffer[BUFFER_SIZE];
        ssize_t count;
        
        while (true) {
            count = recvfrom(fd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&client_addr, &client_len);
            
            if (count == -1) {
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    perror("recvfrom");
                }
                break;
            }
            
            total_clients_connected++;
            buffer[count] = '\0';
            std::string message(buffer, count);
            
            std::cout << "UDP message from " << inet_ntoa(client_addr.sin_addr) << ":" 
                      << ntohs(client_addr.sin_port) << ": " << message << std::endl;
            
            if (message.rfind('/', 0) == 0) {
                std::string response = handle_command(message, shutdown_requested);
                
                if (shutdown_requested) {
                    std::cout << "/shutdown command received via UDP. Shutting down." << std::endl;
                    return;
                }
                if (!response.empty()) {
                    sendto(fd, response.c_str(), response.length(), 0, (struct sockaddr *)&client_addr, client_len);
                }
            } else {
                // Зеркалирование
                sendto(fd, buffer, count, 0, (struct sockaddr *)&client_addr, client_len);
            }
        }
    }