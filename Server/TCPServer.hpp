#ifndef TCPServer_h
#define TCPServer_h

#include "Server.hpp"

class TCPServer : public Server {
private:
    int _newsock;
public:
    TCPServer(int domain);
    TCPServer(int domain, int port);
    ~TCPServer();

    void setup()                                    override;
    std::string  recv()                             override;
    std::string  recv(int size)                     override;
    std::string recvuntil(const std::string& token) override;
    int  send(const std::string& msg)               override;
    void serve(void (*handler)(Server*))            override;
    char* get_msg();
};
#endif /* TCPServer_h */
