#include "TcpClient.h"

TcpClient::TcpClient(int client_fd, int epoll_fd) : SocketWrapper(client_fd), epoll_fd(epoll_fd) {

    if(set_nonblocking(client_fd) == -1){
        throw std::runtime_error("Failed to set non-blocking mode for client.");
    }

    total_clients_connected++;
    current_tcp_clients++;

    epoll_event ev;
    // EPOLLIN ~ready read | EPOLLET ~Edge-Triggered | EPOLLRDHUP ~close
    ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP; 
    ev.data.fd = fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        throw std::runtime_error("epoll_ctl: client_fd failed");
    }
}

TcpClient::~TcpClient() {
    if (fd != -1) {
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
        current_tcp_clients--;
    }
}

bool TcpClient::handle_data(bool& shutdown_requested){
    char buffer[BUFFER_SIZE];
    ssize_t count;

    while (true)
    {
        count = recv(fd, buffer, BUFFER_SIZE -1, 0);
        
        if(count == -1){
            if(errno == EAGAIN || errno == EWOULDBLOCK){
                return true;
            }
            perror("recv");
            return false;
        }

        if(count == 0){
            return false;
        }

        buffer[count] = '\0';
        std::string message(buffer, count);

        if(message.find('/') == 0){
            std::string response = handle_command(message, shutdown_requested);
            
            if (shutdown_requested) {
                return false;
            }

            if(!response.empty()){
                response += "\n";
                send(fd, response.c_str(), response.length(), 0);
            }
        } 
        else{
            // есди это не команда - отправить то, что пришло
            send(fd,buffer,count,0);
        }

        
    }
    
}
