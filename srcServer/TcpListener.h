#pragma once

#include "SocketWrapper.h"
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <cstring>
#include <cstdio>
#include <stdexcept>

class TcpListener : public SocketWrapper{
public:
    TcpListener(int port, int epoll_fd);
};