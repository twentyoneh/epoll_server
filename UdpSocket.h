#pragma once

#include "SocketWrapper.h"
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class UdpSocket : public SocketWrapper{
public:
    UdpSocket (int port, int epoll_fd);
    void handle_data(struct sockaddr_in& client_addr, socklen_t& client_len, bool& shutdown_request);
};

