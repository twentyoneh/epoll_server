// stress_client.cpp - демонстрирует масштабируемость
#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8080
#define NUM_CLIENTS 1000
#define MESSAGES_PER_CLIENT 10

std::atomic<int> clients_connected(0);
std::atomic<int> messages_sent(0);
std::atomic<int> messages_received(0);

void client_thread(int id) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Client " << id << " failed to connect" << std::endl;
        return;
    }
    
    clients_connected++;
    
    for (int i = 0; i < MESSAGES_PER_CLIENT; i++) {
        std::string msg = "Client_" + std::to_string(id) + "_Message_" + std::to_string(i);
        
        send(sock, msg.c_str(), msg.length(), 0);
        messages_sent++;
        
        char buffer[1024];
        int len = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (len > 0) {
            messages_received++;
        }
        
        usleep(1000);
    }
    
    close(sock);
}

int main() {
    std::cout << "=== Epoll Server Stress Test ===" << std::endl;
    std::cout << "Starting " << NUM_CLIENTS << " concurrent clients..." << std::endl;
    
    std::vector<std::thread> threads;
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < NUM_CLIENTS; i++) {
        threads.emplace_back(client_thread, i);
        if (i % 100 == 0) {
            std::cout << "Started " << i << " clients..." << std::endl;
        }
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "\n=== Results ===" << std::endl;
    std::cout << "Total time: " << duration.count() << " ms" << std::endl;
    std::cout << "Clients connected: " << clients_connected << std::endl;
    std::cout << "Messages sent: " << messages_sent << std::endl;
    std::cout << "Messages received: " << messages_received << std::endl;
    std::cout << "Throughput: " << (messages_sent * 1000.0 / duration.count()) << " msg/sec" << std::endl;
    
    return 0;
}