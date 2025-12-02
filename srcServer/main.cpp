#include "EpollServer.h"
#include <iostream>
#include <cstdlib>

int main() {
    try {
        EpollServer server;
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}