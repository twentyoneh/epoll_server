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
    time_t rawtime = time(nullptr);
    struct tm* timeinfo = localtime(&rawtime); 
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