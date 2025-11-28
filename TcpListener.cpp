#include "TcpListener.h"

TcpListener::TcpListener(int port, int epoll_fd){
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0) throw std::runtime_error("socket tcp failed");
    
    int opt = 1;
    setsockopt(fd, SOL_SOCKET,SO_REUSEADDR, &opt, sizeof(opt));

    if(set_nonblocking(fd) == -1){
        close(fd);
        throw std::runtime_error("Filed to set non-blocing mode for TCP");
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if(bind(fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        close(fd);
        throw std::runtime_error("bind tcp failed");
    }
    if(listen(fd, SOMAXCONN) < 0){
        close(fd);
        throw std::runtime_error("listen tcp failed");
    }

    epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = fd;
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1){
        close(fd);
        throw std::runtime_error("epoll_ctl: tcp_listen_fd failed");
    }
    std::cout << "TCP listening on port " << port << std::endl;
}