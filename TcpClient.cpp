#include "TcpClient.h"
#include <iostream>
#include <string>

TcpClient::TcpClient(int client_fd, int epoll_fd) : SocketWrapper(client_fd), epoll_fd(epoll_fd) {

    if(set_nonbloking(client_fd) == -1){
        throw std::runtime_error("Failed to set non-blocking mode for client.");
    }

    total_clients_connected++;
    current_tcp_clients++;

    epoll_event ev;
    // EPOLLIN ~ready read | EPOLLET ~Edge-Triggered | EPOLLRDHUP ~close
    ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP; 
    ev.data.fd = fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        throw std::runtime_error("epoll_ctl: client_fd failed");
    }
}

TcpClient::~TcpClient() {
    if (fd != -1) {
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
        current_tcp_clients--;
    }
}