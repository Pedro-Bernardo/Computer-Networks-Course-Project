#include "../Client/UDPClient.hpp"
#include "../Server/UDPServer.hpp"
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <vector>
#include <arpa/inet.h>
#include <unistd.h>
#include "Dir.hpp"
#include "File.hpp"
#include "User.hpp"
#define MAXLINE 128

extern int optdin;

UDPClient* client;
//lima

void clear_stdin() {
	char c;
	while((c = getchar()) != '\n');
}

void sig_int_handler(int s) {
	std::string response;
	client->send("UNR 127.0.0.1 58002\n");
	response = client->recv();
	if(response == "UAR OK\n") {
		exit(0);
	}

	return;
}

void handler_udp(Server* server) {
	while(true) {
		std::vector<std::string> in;
		std::cout << "Running the UDP handler" << std::endl;
		std::string received = server->recvuntil("\n");
		std::string token, cmd;
		std::cout << received << std::endl;

		std::istringstream sstrm(received);

		while(std::getline( sstrm, token, ' ')) {
			std::cout << token << std::endl;
			in.push_back(token);
		}

		cmd = in[0];

		if( cmd == "LSU" ) {
			
		}
	}
	
}	

int main(int argc, char* argv[]) {
    // first step is to register in the CS
	struct sigaction sigint_action;
	struct hostent *h;
	struct in_addr *a;
	char buffer[MAXLINE + 1];
	std::string ip;
	std::string BSport = "59000";
	std::string CSname = "localhost";
	std::string CSport = "58012";
	int csport;
	int bsport;
	size_t size;
	char opt;
	client = new UDPClient(AF_INET, 58001);
	UDPServer* server;

	/* check cmd line argument */
	while ((opt = getopt(argc, argv, "bnp")) != -1) {
        switch (opt) {
        case 'b': BSport = argv[optind]; break;
        case 'n': CSname = argv[optind]; break;
        case 'p': CSport = argv[optind]; break;
        }
    }

	std::cout << BSport << std::endl;
	std::cout << CSname << std::endl;
	std::cout << CSport << std::endl;

	bsport = std::stoi(BSport, &size);

	/* check user bsport */
	if((int)size < (int)(BSport.length()-1) ) {
		std::cerr << "invalid argument - BSport" << std::endl;
		exit(1);
	}

	csport = std::stoi(CSport, &size);

	/* check user bsport */
	if((int)size < (int)(CSport.length()-1)) {
		std::cerr << "invalid argument - CSport" << std::endl;
		exit(1);
	}

	/* get my ip */
	if(gethostname(buffer, 128) == -1)
		printf("error: %s\n", strerror(errno));

	if( (h = gethostbyname(buffer)) == NULL)
		exit(1);

	a = (struct in_addr*)h->h_addr_list[0];
	ip = inet_ntoa(*a);


   /* get ip addr in some way... */
    client->set_connection();
    client->send("REG " + ip + " " + std::to_string(bsport));
    std::string response = client->recv();

	if(response != "RGR OK\n") {
		std::cerr << "Error registering BS server" << std::endl;
 	}

    /* install the SIGINT signal handler */
    sigemptyset(&sigint_action.sa_mask);
    sigint_action.sa_flags = 0;
    sigint_action.sa_handler = sig_int_handler;
    if(sigaction(SIGINT, &sigint_action, NULL) == -1) {
		printf("Error assigning SIGINT handler\n");
		exit(1);
	}

	
	server = new UDPServer(AF_INET, bsport);
	server->setup();
	server->serve(handler_udp);
	


	return 0;
}
