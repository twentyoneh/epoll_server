#pragma once

#include "SocketWrapper.h"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <errno.h>

class TcpClient : public SocketWrapper{
private:
    int epoll_fd;
public:
    TcpClient(int client_fd, int epoll_fd);
    ~TcpClient() override;
    TcpClient(const TcpClient&) = delete;
    TcpClient& operator=(const TcpClient&) = delete;
    
    TcpClient(TcpClient&&) = default; 
    TcpClient& operator=(TcpClient&&) = default; 
    
    bool handle_data(bool& shutdown_requested);
};