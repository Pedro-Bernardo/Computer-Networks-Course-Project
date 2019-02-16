#ifndef UDPClient_h
#define UDPClient_h

#include "Client.hpp"

class UDPClient : public Client {
public:
    UDPClient(int domain, int port);
    UDPClient(int domain, int port, const std::string& host);
    ~UDPClient();
    std::string  recv()                         override;
    std::string recv(int size)                 override;
    std::string  recvuntil(const std::string& token) override;
    int  send(const std::string& msg)   override;
    void set_connection()                      override;
};
#endif /* UDPClient_h */
