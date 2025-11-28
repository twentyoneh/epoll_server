#pragma once

#include<string>
#include <fcntl.h>
#include <stdio.h>
#include <algorithm>
#include <ctime>

extern long total_clients_connected;
extern int current_tcp_clients;

const int BUFFER_SIZE = 1024;
const int PORT = 8080;

int set_nonbloking(int fd);
std::string handle_command(const std::string& inp, bool& shutdown_request);
