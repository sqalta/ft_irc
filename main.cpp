#include "Server/Server.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cerr << "USAGE: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    int port = atoi(argv[1]);
    if (port < 1024 || port > 65535) {
        std::cerr << "ERROR: Invalid port number (1024-65535)" << std::endl;
        return 1;
    }

    try {
        Server Server(port, argv[2]);
    }
    catch (const std::exception& e) {
        std::cerr << "Server hatası: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
