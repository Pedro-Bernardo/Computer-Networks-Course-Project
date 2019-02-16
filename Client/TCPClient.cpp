#include "Client.hpp"
#include "TCPClient.hpp"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int errno;

TCPClient::TCPClient(int domain, int port, const std::string& host) {
    _port = port;
    _host = host;
    if ((_sock = socket(domain, SOCK_STREAM, 0)) == -1) {
		perror("Could not create socket");
		exit(1);
	}

	if(!(_hostptr = gethostbyname(_host.c_str()))) {
		printf("Error resolving hostname: %s\n", strerror(errno));
		exit(1);
	}

    memset((void*)&_serveraddr,(int)'\0',sizeof(_serveraddr));
	_serveraddr.sin_family = domain;
	_serveraddr.sin_addr.s_addr = ((struct in_addr *)(_hostptr->h_addr_list[0]))->s_addr;
	_serveraddr.sin_port = htons((u_short) _port);
	_addrlen = sizeof(_serveraddr);

}

TCPClient::TCPClient(int domain, int port) : TCPClient(domain, port, "localhost") {}

TCPClient::~TCPClient() {
    close(_sock);
}

std::string TCPClient::recv()  {
    this->recv(79);
    return _buffer;
}

std::string TCPClient::recv(int size)  {
     /* temporary magic number */
    int n = read(_sock, _buffer, size);
    _buffer[n] = '\0';

    return _buffer;
}

std::string TCPClient::recvuntil(const std::string& token) {
    std::string aux;
    std::string received;
    while((aux = this->recv(1)) != token){
        received += aux;
    }

    return received;
}

int TCPClient::send(const std::string& msg) {
    // std::cout << msg.length() << std::endl;
    /*  check the size of the input and send it to the server 
        so it can receive it propperly
    */
    char size[5];
    sprintf(size, "%d", (int)msg.length());
    // printf("Size of str %s -> %s\n", size, msg.c_str());

    //write(_sock, size, 4);

    // then send the real message
    return write(_sock, msg.c_str(), msg.length());
}


void TCPClient::set_connection() {
    /* main server loop */
    while(connect(_sock, (struct sockaddr*) &_serveraddr, sizeof(_serveraddr)) == -1);
    
    //send("OLAAA\n");
}

void TCPClient::bye() {
    close(_sock);
}
