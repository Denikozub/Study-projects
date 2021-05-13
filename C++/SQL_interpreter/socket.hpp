#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <cstring>
#include <cerrno>
using std::string;

#define CHECK_SOCKET(res) if (res < 0) throw (string)"error: " + strerror(errno) + "\n"
#define MAX_LEN 1024

class Socket {
public:
    virtual void connect() = 0;
    virtual void send(string) = 0;
    virtual string receive() = 0;
    virtual ~Socket() = default;
};

#endif
