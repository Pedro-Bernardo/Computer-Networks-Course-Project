#ifndef UDPServer_h
#define UDPServer_h

#include "Server.hpp"

class UDPServer : public Server {
public:
    UDPServer(int domain);
    UDPServer(int domain, int port);
    ~UDPServer();
    
    void setup()                        override;
    std::string recv()               override;
    std::string recv(int size)       override;
    std::string recvuntil(const std::string& token) override;
    int  send(const std::string& msg)   override;
    void serve(void (*handler)(Server*))                        override;
    std::string get_ip() const;
};
#endif /* UDPServer_h */