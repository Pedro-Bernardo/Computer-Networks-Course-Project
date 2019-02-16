#include "UDPServer.hpp"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

extern int errno;

UDPServer::UDPServer(int domain, int port) {
    _sock = socket(domain, SOCK_DGRAM, 0);
    _port = port;
    
    memset((void*)&_buffer,(int)'\0',sizeof(_buffer));
    memset((void*)&_serveraddr,(int)'\0',sizeof(_serveraddr));
    _serveraddr.sin_family = domain;
    _serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    _serveraddr.sin_port=htons((u_short)_port);
    _hostptr = gethostbyname("localhost");
}

UDPServer::UDPServer(int domain) : UDPServer(domain, 58001) {} /* add group name */

UDPServer::~UDPServer() {
    close(_sock);
    std::cout << "Destroying UDPServer" << std::endl;

}

void UDPServer::setup() {
    bind(_sock, (struct sockaddr*) &_serveraddr, sizeof(_serveraddr));
    _addrlen = sizeof(_clientaddr);
}

std::string UDPServer::recv()  {
    this->recv(79);
    return _buffer;
}

std::string UDPServer::recv(int size)  {
    int ret = recvfrom(_sock, _buffer, size,
                        0, (struct sockaddr*) &_clientaddr,
                        &_addrlen);

    _hostptr = gethostbyaddr((char*) &_clientaddr.sin_addr, sizeof(struct in_addr), AF_INET);
    printf("sent by [%s:%hu]\n", inet_ntoa(_clientaddr.sin_addr), ntohs(_clientaddr.sin_port));
    printf("sent by [%s:%hu]\n", _hostptr->h_name,ntohs(_clientaddr.sin_port));
    

    if( ret == -1 ) {
        perror("recvfrom call failed");
    } else {
        _buffer[ret] = '\0';
        printf("RECEIVED: %s\n", _buffer);
        //return ret;
    }

    return _buffer;
}

std::string UDPServer::recvuntil(const std::string& token) {
    std::string aux;
    std::string received;
    while((aux = this->recv(1)) != token){
        received += aux;
    }

    return received;
}

int UDPServer::send(const std::string& msg) {
    int ret = sendto(_sock, msg.c_str(), msg.length(), 
                    0, (struct sockaddr*) &_clientaddr, 
                    _addrlen);

    if( ret == -1 ) {
        perror("sendto call failed");
        return -1;
    } else {

        return ret;
    }
}


void UDPServer::serve(void (*handler)(Server*)) {
    /* main server loop */
    std::cout << "Serving UDP..." << std::endl;
    handler(this);
    
}

std::string UDPServer::get_ip() const {
    std::cout << inet_ntoa(_serveraddr.sin_addr) << std::endl;
    return inet_ntoa(_serveraddr.sin_addr);
}