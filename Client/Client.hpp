#ifndef Client_h
#define Client_h

#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

class Client {
protected:
    /* connection */
    int _sock;
    int _port;
    std::string _host;
    struct hostent *_hostptr;
    struct sockaddr_in _serveraddr, _clientaddr;
    socklen_t _addrlen;
    char _buffer[80];

public:
    virtual ~Client() {}
    virtual std::string recv() = 0;
    virtual std::string recv(int size) = 0;
    virtual std::string  recvuntil(const std::string& token) = 0;
    virtual int send(const std::string& msg) = 0;
    virtual void set_connection() = 0;
};
#endif /* Client_h */
