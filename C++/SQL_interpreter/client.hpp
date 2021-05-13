#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "socket.hpp"

class Client : public Socket {
    int sock_fd;
    struct sockaddr_un sa;
public:
    Client(string);
    void connect();
    void send(string);
    string receive();
    ~Client();
};

#endif
