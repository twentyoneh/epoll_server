#pragma once

#include "SocketWrapper.h"
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>

class TcpListener : public SocketWrapper{
public:
    TcpListener(int port, int epoll_fd);
};