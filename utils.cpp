#include "utils.h"

long total_clients_connected = 0;
int current_tcp_clients = 0;

int set_nonblocking(int fd){
    int flags = fcntl(fd, F_GETFL, 0); // В каком состоянии дескриптор файла
    if(flags == -1){
        perror("fcntl F_GETFL: bad file descriptor");
        return -1;
    }
    if(fcntl(fd,F_SETFL,flags | O_NONBLOCK) == -1){ // Установка неблокирующего режима для fd
        perror("fcntl F_SETFL O_NONBLOCK");
        return -1;
    }
    return 0;
}

std::string handle_command(const std::string& inp, bool& shutdown_request){
    std::string command = inp;
    command.erase(std::remove_if(command.begin(),command.end(), ::isspace),command.end());

    if(command == "/time"){
        time_t t = time(NULL);
        struct tm* tm = localtime(&t);
        char time_str[64];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm);
        return std::string(time_str);
    }
    else if (command == "/stats") {
        return "Total connected clients: " + std::to_string(total_clients_connected) + 
               ", Currently connected TCP clients: " + std::to_string(current_tcp_clients);
    } 
    else if (command == "/shutdown") {
        shutdown_request = true;
        return ""; // Ответ не нужен
    }
    else {
        return "Unknown command.";
    }
}