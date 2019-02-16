#ifndef TCPClient_h
#define TCPClient_h

#include "Client.hpp"

class TCPClient : public Client {
public:
    TCPClient(int domain, int port);
    TCPClient(int domain, int port, const std::string& host);
    ~TCPClient();

    std::string recv()                         override;
    std::string recv(int size)                 override;
    std::string  recvuntil(const std::string& token) override;
    int  send(const std::string& msg)          override;
    void set_connection()                      override;
    // test
    void bye();
};
#endif /* TCPClient_h */
