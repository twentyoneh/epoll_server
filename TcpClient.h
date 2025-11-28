#pragma once

#include "SocketWrapper.h"
#include <sys/epoll.h>
#include <sys/socket.h>

class TcpClient : public SocketWrapper{
private:
    int epoll_fd;
public:
    TcpClient(int client_fd, int epoll_fd);
    ~TcpClient() override;
    bool handle_data(bool& shutdown_requested);
};