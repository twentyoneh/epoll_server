#pragma once
#include <unistd.h>
#include <iostream>
#include <stdexcept>
#include "utils.h"

class SocketWrapper {
protected:
    int fd = -1;
public:
    SocketWrapper() = default;
    explicit SocketWrapper(int socket_fd) : fd(socket_fd) {
        if (fd < 0) throw std::runtime_error("Invalid file descriptor passed.");
    }
    
    virtual ~SocketWrapper() {
        if (fd != -1) {
            close(fd);
        }
    }
    
    SocketWrapper(const SocketWrapper&) = delete;
    SocketWrapper& operator=(const SocketWrapper&) = delete;
    
    SocketWrapper(SocketWrapper&& other) : fd(other.fd) {
        other.fd = -1;
    }
    SocketWrapper& operator=(SocketWrapper&& other){
        if (this != &other) {
            if (fd != -1) close(fd);
            fd = other.fd;
            other.fd = -1;
        }
        return *this;
    }
    
    int get_fd() const { return fd; }
};