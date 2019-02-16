#ifndef Server_h
#define Server_h

#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

class Server {
protected:
    /* connection */
    int _sock;
    int _port;
    /* entry in the hosts database */
    struct hostent *_hostptr;
    struct sockaddr_in _serveraddr, _clientaddr;
    socklen_t _addrlen;
    socklen_t _clientlen;
    char _buffer[80];

public:
    virtual ~Server() {}
    virtual void setup() = 0;
    virtual std::string recv() = 0;
    virtual std::string recv(int size) = 0;
    virtual std::string recvuntil(const std::string& token) = 0;
    virtual void serve(void (*handler)(Server*)) = 0;
    virtual int send(const std::string& msg) = 0;


//    Server(int domain, int type) = 0;

};
#endif /* Server_h */
