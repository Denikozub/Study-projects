#ifndef SERVER_HPP
#define SERVER_HPP

#include "socket.hpp"

class Server : public Socket {
    int sock_fd, connect_fd;
    struct sockaddr_un sa;
public:
    Server(string);
    void connect();
    void send(string);
    string receive();
    ~Server();
};

#endif
