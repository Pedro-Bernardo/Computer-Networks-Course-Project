#include "Client.hpp"
#include "UDPClient.hpp"
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

UDPClient::UDPClient(int domain, int port, const std::string& host) {
    _port = port;
    _host = host;
    if ((_sock = socket(domain, SOCK_DGRAM, 0)) == -1) {
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

UDPClient::UDPClient(int domain, int port) : UDPClient(domain, port, "localhost") {}

UDPClient::~UDPClient() {
    close(_sock);
    std::cout << "Destroying UDPClient" << std::endl;
}

std::string UDPClient::recv() {
    return this->recv(79);
}
std::string UDPClient::recv(int size)  {
    int ret = recvfrom(_sock, _buffer, size,
                        0, (struct sockaddr*) &_serveraddr,
                        &_addrlen);

    if( ret == -1 ) {
        perror("recvfrom call failed");
    } else {
        _buffer[ret] = '\0';
        printf("RECEIVED: %s\n", _buffer);
        //return ret;
    }

    return _buffer;
}

std::string  UDPClient::recvuntil(const std::string& token) {
    std::string aux;
    std::string received;
    while((aux = this->recv(1)) != token){
        received += aux;
    }

    return received;
}

int UDPClient::send(const std::string& msg) {
    int ret = sendto(_sock, msg.c_str(), msg.length(), 0, 
                    (struct sockaddr*) &_serveraddr,
                    _addrlen);

    if( ret == -1 ) {
        perror("sendto call failed");
        return -1;
    }

    return ret;
}


void UDPClient::set_connection() {
    /* main server loop */
}
