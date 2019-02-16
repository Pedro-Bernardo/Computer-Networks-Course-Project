#include "Server.hpp"
#include "TCPServer.hpp"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int errno;

TCPServer::TCPServer(int domain, int port) {
    _sock = socket(domain, SOCK_STREAM, 0);
    _port = port;

    memset((void*)&_buffer,(int)'\0',sizeof(_buffer));
    memset((void*)&_serveraddr,(int)'\0',sizeof(_serveraddr));
    _serveraddr.sin_family = domain;
    _serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    _serveraddr.sin_port=htons((u_short)_port);
}

TCPServer::TCPServer(int domain) : TCPServer(domain, 58000) {} /* add group name */

TCPServer::~TCPServer() {
    close(_sock);
    std::cout << "Destroying TCPServer" << std::endl;
}

void TCPServer::setup() {
    bind(_sock, (struct sockaddr*) &_serveraddr, sizeof(_serveraddr));
    std::cout << inet_ntoa(_serveraddr.sin_addr) << std::endl;
    listen(_sock, 5);
}

std::string TCPServer::recv()  {
    this->recv(79);
    return _buffer;
}

std::string TCPServer::recv(int size)  {
     /* temporary magic number */
    int n = read(_newsock, _buffer, size);
    std::cout << "RECEIVED " << n << " bytes!" << std::endl;
    if(n == 0) return "off";
    _buffer[n] = '\0';
    std::cout << _buffer << std::endl;
    return _buffer;
}

std::string TCPServer::recvuntil(const std::string& token) {
    std::string aux;
    std::string received;
    while((aux = this->recv(1)) != token){
        received += aux;
    }

    return received;
}


int TCPServer::send(const std::string& msg) {
    std::cout << "Sending " << msg << std::endl;
    return write(_newsock, msg.c_str(), msg.length());
}

char* TCPServer::get_msg() {
    return _buffer;
}

void TCPServer::serve(void (*handler)(Server*)) {
    int child_pid;
    /* main server loop */
    std::cout << "Serving TCP..." << std::endl;

    while(true) {
         _clientlen = sizeof(_clientaddr);

        /* single client only */
        _newsock = accept(_sock, (struct sockaddr*) &_clientaddr, &_clientlen);

        if(_newsock < 0) perror("error establishing connection to client - accept");

        if((child_pid = fork()) < 0) perror("error creating handler process");
        else if (child_pid == 0) {
            /* child process: close original socket */
            close(_sock);
            handler(this);
            exit(0);
        }

        /* parent process */
        close(_newsock);
    }
}
