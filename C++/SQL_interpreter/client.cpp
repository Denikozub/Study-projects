#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "client.hpp"
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::exception;


Client::Client(string address_name) {
    CHECK_SOCKET((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)));
    sa.sun_family = AF_UNIX;
    strcpy(sa.sun_path, address_name.c_str());
}

void Client::connect() {
    while (::connect(sock_fd, (struct sockaddr*)&sa, sizeof(sa)) != 0)
        sleep(1);
}

void Client::send(string message) {
    CHECK_SOCKET(write(sock_fd, message.c_str(), message.length()));
}

string Client::receive() {
    char buf[MAX_LEN] = {0};
    CHECK_SOCKET(read(sock_fd, buf, MAX_LEN));
    return (string)buf;
}

Client::~Client() {
    shutdown(sock_fd, 2);
    close(sock_fd);
}

int main ()
{
    try {
        Client c("My_socket");
        c.connect();
        string message, answer;
        do {
            std::getline(std::cin, message);
            c.send(message);
            cout << "_______________________________________________" << endl;
            answer = c.receive();
            cout << answer;
            if (answer.find("error") != answer.npos)
                break;
            cout << "_______________________________________________\n" << endl;
        } while (message != "stop" and !std::cin.eof());
        
    } catch (string message) {
        cerr << message << endl;
    } catch (exception& e) {
        cerr << typeid(e).name() << ": " << e.what() << endl;
    }
    
    return 0;
}
