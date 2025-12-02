#pragma once

#include <vector>
#include <sys/epoll.h>
#include <stdexcept>

#include "TcpListener.h"
#include "UdpSocket.h"
#include "TcpClient.h"

const int MAX_EVENTS = 64;

class EpollServer {
private:
    time_t rawtime = time(nullptr);
    struct tm* timeinfo = localtime(&rawtime); 
    int epoll_fd;
    TcpListener* tcp_listener = nullptr;
    UdpSocket* udp_socket = nullptr;
    std::vector<TcpClient> tcp_clients;
    bool shutdown_flag = false;
    
    void handle_new_tcp_connections();
    void handle_tcp_client_data(int fd, uint32_t events);
    
public:
    EpollServer();
    ~EpollServer();
    void run();
};