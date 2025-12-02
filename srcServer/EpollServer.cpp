#include "EpollServer.h"

EpollServer::EpollServer(){
    epoll_fd = epoll_create1(0);
    if(epoll_fd == -1){
        throw std::runtime_error("epoll_create1 failed");
    }

    tcp_listener = new TcpListener(PORT, epoll_fd);
    udp_socket = new UdpSocket(PORT, epoll_fd);
}

EpollServer::~EpollServer() {
    // Деструкторы классов SocketWrapper и TcpClient закроют все сокеты
    delete tcp_listener;
    delete udp_socket;
    // Вектор tcp_clients очистится автоматически, закрывая все сокеты
    if (epoll_fd != -1) {
        close(epoll_fd);
        std::cout << "Epoll closed." << std::endl;
    }
}

void EpollServer::run(){
    epoll_event events[MAX_EVENTS];
    std::cout << "info: " << asctime(timeinfo) << "Server is running on port " << PORT << std::endl;

    while(!shutdown_flag){
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        if(nfds == -1){
            if(errno == EINTR) continue;
            perror("epoll_wait");
            break;
        }

        for(int i = 0; i < nfds; ++i){
            int current_fd = events[i].data.fd;

            // TCP
            if(current_fd == udp_socket->get_fd()){
                handle_new_tcp_connections();
            }
            // UDP
            else if(current_fd == udp_socket->get_fd()){
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                udp_socket->handle_data(client_addr,client_len,shutdown_flag);
                if(shutdown_flag) break;
            }
            else{
                handle_tcp_client_data(current_fd,events[i].events);
                if(shutdown_flag) break;
            }
        }
        if(shutdown_flag) break;
    }
    std::cout << "info: " << asctime(timeinfo) << "Server shutting down cleanly." << std::endl;
}

void EpollServer::handle_new_tcp_connections(){
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    while (true) {
        int client_fd = accept(tcp_listener->get_fd(), (struct sockaddr *)&client_addr, &client_len);
        if (client_fd == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) break;
            perror("accept");
            break;
        }

        try {
            tcp_clients.emplace_back(client_fd, epoll_fd);
            std::cout << "info: " << asctime(timeinfo) << "New TCP client connected from " << inet_ntoa(client_addr.sin_addr) 
                      << ":" << ntohs(client_addr.sin_port) << " (FD: " << client_fd << ")" << std::endl;
        } 
        catch (const std::exception& e) {
            std::cerr << "info: " << asctime(timeinfo) << "Error accepting client: " << e.what() << std::endl;
            close(client_fd);
        }
    }
}

void EpollServer::handle_tcp_client_data(int fd, uint32_t events){
    auto it = std::find_if(tcp_clients.begin(), tcp_clients.end(), 
                           [fd](const TcpClient& c){ return c.get_fd() == fd; });
    if (it == tcp_clients.end()) return;

    if (events & (EPOLLHUP | EPOLLERR | EPOLLRDHUP)) {
        std::cout << "info: " << asctime(timeinfo) << "Client disconnected (FD: " << fd << ").\n";
        tcp_clients.erase(it);
        return;
    }
    
    if (events & EPOLLIN) {
        if (!it->handle_data(shutdown_flag)) {
            std::cout << "info: " << asctime(timeinfo) << "Closing client connection (FD: " << fd << ").\n";
            tcp_clients.erase(it);
        }
    }
}