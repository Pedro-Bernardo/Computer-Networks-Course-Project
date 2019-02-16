#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <list>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include "utils.hpp"
#include "../Client/UDPClient.hpp"
#include "User.hpp"
std::list<BServer*>  servers;
char server_db[] = "servers.txt";
User* user = NULL;


bool validate_user(Server* server) {
	/*
		responder ao pedido inicial de login e retornar
		true se a resposta ao cliente for AUR NEW ou AUR OK
		else false

		Para isso, verificar as informacoes dos users ou adicionar
	*/
	std::string::size_type size;
	std::string fname;
	std::string passwd;
	FILE* fd;
	int id;
	std::string in = server->recv(4);
	if(in == "off"){
		std::cout << "Disconnected" << std::endl;
		return false;
	}
	std::cout << in << std::endl;


	/* check command */
	if(in != "AUT ") {
		server->recv();
		server->send("ERR\n");
		return false;
	}

	in = server->recv(6);
	/* convert id into int */
	id = std::stoi(in, &size);

	/* check user id */
	if((int)size < (int)(in.length() - 1) || in[(int)size] != ' ') {
		server->recv();
		server->send("ERR\n");
		return false;
	}

	/* create file name */
	fname = std::to_string(id);
	std::cout << "FILE NAME " << fname << std::endl;

	/* receive passwd */
	in = server->recv();

	/* check input format */
	if(in[in.length() - 1] != '\n' || in.length() < 9){
		server->send("ERR\n");
		return false;
	}

	/* testar passwd */
	passwd = in.substr(0, in.length() - 1);


	fd = fopen(fname.c_str(), "r");
	if( fd == NULL ){
		/* if user isnt registered, create a new user and write it to the db file */
		std::cout << "NEW USER" << std::endl;
		user = new User(id, passwd);
		server->send("AUR NEW\n");
		write_user();
		return false;
	}

	/* if user is registered, check the passwd first (read user if needed1) */
	if (user == nullptr) read_user(fname);
	bool correct_passwd = user->get_passwd() == passwd;

	if(!correct_passwd) {
		server->send("AUR NOK\n");
		std::cout << "Wrong passwd" << std::endl;
		return false;
	} else {
		if(!user->is_logged_in()) {
			server->send("AUR OK\n");
			user->login();
			return false;
		} else {
			server->send("AUR OK\n");
			return true;
		}
	}

}
void backup(Server *server){
	std::string received;
	std::string dirname;
	std::string token,token2;
	std::string ip;
	std::vector<std::string> in;
	int dirsize;
	int i;

	received = server->recvuntil("\n");

	std::istringstream sstrm(received);
	while(std::getline(sstrm, token, ' ')){
		in.push_back(token);
	}

	dirname = in[0];
	dirsize = std::stoi(in[1], nullptr);
	in.clear();

	std::vector< std::vector<std::string> > files(dirsize);
	for(i = 0; i < dirsize; i++) {
		received = server->recvuntil("\n");
		std::istringstream sstrm(received);
		while(std::getline(sstrm, token, ' ')){
			files[i].push_back(token);
		}
	}
	/* escolher bs server e pedir lhe para registar o user */
	BServer* chosen = user->checkDir(dirname);
	if(!chosen){
		load_bservers();
		for(BServer* srv : servers) {
			if(srv->is_available()) {
				chosen = srv;
				break;
			}
		}
		chosen->inc_count();
		user->add_backup(dirname,chosen);

		write_user();
		update_bserver_db();

		UDPClient client(AF_INET,chosen->get_port(),chosen->get_ip());
		client.set_connection();
		std::string aux;
		aux = "LSU " + std::to_string(user->get_id()) + " " + user->get_passwd() + "\n";
		client.send(aux);
		client.recv(4);
		/* porque nao recvuntil? agora ja podemos */
		aux = client.recv();
		if(aux != "OK\n"){
			/* pk write user??? , ele n fez backup */
				write_user();
				delete user;
				return;
		}
		std::string out = "BKR " + chosen->get_ip() + " " + std::to_string(chosen->get_port()) + " " + std::to_string(dirsize) + "\n";
		server->send(out);
		std::cout << out;
		for(int i = 0; i < dirsize; i++) {
			std::string response = files[i][0] + " " + files[i][1] + " " + files[i][2] + " " + files[i][3]+"\n";
			server->send(response);
		}
	}
	else{
		/* abrir udp */
		UDPClient client(AF_INET, chosen->get_port(), chosen->get_ip());
		client.set_connection();
		std::string aux;
		aux = "LSF " + std::to_string(user->get_id()) + " " + dirname + "\n";
		client.send(aux);
		client.recv(4);
		received = client.recvuntil("\n");
		in.clear();
		std::istringstream sstrm(received);
		while(std::getline(sstrm, token, ' ')){
			in.push_back(token);
		}
		std::string response = "";
		for(int i = 0; i < (int)in.size(); i += 4){
			bool flag = false;
			for(int e = 0; e < dirsize; e++) {
				if(files[e][0] == in[i]){
					flag = true;
					if(files[e][1]!=in[i+1] || files[e][2]!=in[i+2]){
						response += files[i][0] + " " + files[i][1] + " " + files[i][2] + " " + files[i][3]+"\n";
					}
				}
			}
			if(!flag){
				response += files[i][0] + " " + files[i][1] + " " + files[i][2] + " " + files[i][3]+"\n";
			}
		}
		std::string out="BKR " + chosen->get_ip() + " " + std::to_string(chosen->get_port()) + " " + std::to_string(dirsize) + "\n";
		server->send("BKR " + chosen->get_ip() + " " + std::to_string(chosen->get_port()) + " " + std::to_string(dirsize) + "\n");
		std::cout<<out;
		server->send(response);
	}
}
void handler_user(Server* server) {
	std::cout << "Connected" << std::endl;
	if(validate_user(server)) {
		/* lê comando */
		std::cout << "HERE" << std::endl;
		std::string cmd = server->recv(4);

		if(cmd == "LSD\n"){
			/* dirlist command */
			std::vector<std::string> dirnames = user->get_dirs();
			int n_dirs = dirnames.size();
			std::string out = "LDR " + std::to_string(n_dirs);
			if(n_dirs == 0) {
				out += "\n";
				server->send(out);
			} else {
				for(std::string dir : dirnames){
					out += " " + dir;
				}
				out += "\n";
				server->send(out);
			}

		} else if (cmd == "BCK ") {
			/* backup command */
			backup(server);
		}
	}

	if(user != nullptr) {
		write_user();
		delete user;
	}
}


void handler_udp(Server* server) {
	/* udp server cycle (always runs) */
	std::cout << "Writing to servers file" << std::endl;
	FILE* fd = fopen(server_db, "r");
	if(fd == nullptr) {
		fd = fopen(server_db, "w");
		fprintf(fd, "BEGIN |\n");
	}
	fclose(fd);

	while(true) {
		struct sockaddr_in test;
		std::vector<std::string> in;
		std::cout << "Running the UDP handler" << std::endl;
		std::string received = server->recv();
		std::string token;
		std::cout << received << std::endl;

		std::istringstream sstrm(received);

		while(std::getline( sstrm, token, ' ')) {
			std::cout << token << std::endl;
			in.push_back(token);
		}

		if(in[0] == "REG") {
			/* check if ip addr is valid */
			std::string::size_type size;
			int port;
			port = std::stoi(in[2], &size);
			std::cout << std::to_string(inet_pton(AF_INET, in[1].c_str(), &(test.sin_addr))) << std::endl;
			/* check user id */
			if(((int)size < (int)(in[2].length() - 1)) || (inet_pton(AF_INET, in[1].c_str(), &(test.sin_addr))) == 0) {
				server->send("RGR ERR\n");
			} else {
				BServer* srv = new BServer(in[1], port);
				server->send("RGR OK\n");
				/* and register the server!!! */

				servers.push_front(srv);

				add_bserver(srv);
			}
			// JUST TO TEST
		} else if(in[0] == "UNR") {
			/* check if ip addr is valid */
			std::string::size_type size;
			int port;
			port = std::stoi(in[2], &size);

			/* check if the port number and the ip addr are in the correct format */
			if(((int)size < (int)(in[2].length() - 1)) || (inet_pton(AF_INET, in[1].c_str(), &(test.sin_addr))) == 0) {
				server->send("UAR ERR\n");
			} else { /* valid data */
				std::string ip = in[1];

				BServer *bsrv = get_bserver(ip, port);

				/* always remove the server for now, but only free if it has zero backups (clients might be pointing to it) */
				servers.remove(bsrv);

				delete bsrv;

				std::cout << "SENDING UAR OK" << std::endl;
				server->send("UAR OK\n");
				update_bserver_db();
			}
		}
		//h=gethostbyaddr((char*)&addr.sin_addr,sizeof(struct in_addr),AF_INET);
	}

}

void sig_int_handler(int s) {
	for(BServer* srv : servers) {
		delete srv;
	}
	servers.clear();
	// the pointers are the same, and they are already deleted (freed)

	if(user != nullptr)
		write_user();

	unlink(server_db);
	exit(0);
}


int main(int argc, char const *argv[])
{
	TCPServer tcp_server(AF_INET, 58002);
	int child_pid;
	// test
	servers.push_front(new BServer("127.0.0.1", 58002));
	update_bserver_db();
	struct sigaction sigint_action;

    /* install the SIGINT signal handler */
    sigemptyset(&sigint_action.sa_mask);
    sigint_action.sa_flags = 0;
    sigint_action.sa_handler = sig_int_handler;
    if(sigaction(SIGINT, &sigint_action, NULL) == -1) {
		printf("Error assigning SIGINT handler\n");
		exit(1);
	}


	if((child_pid = fork()) < 0) exit(0);
	else if (child_pid == 0) {
		UDPServer udp_server(AF_INET, 58001);
		udp_server.setup();
		udp_server.serve(handler_udp);

		exit(0);
	}

	tcp_server.setup();
	tcp_server.serve(handler_user);

	return 0;
}
