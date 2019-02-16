#include "../Client/TCPClient.hpp"
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include "time.h"
// #include <signal.h>
#define MAXLINE 80

using namespace std;

string _id;
string _pass;

bool login(TCPClient* client, const std::string& id, const std::string& pass) {
	string request;
	string response;

	request = "AUT " + id + " " + pass + "\n";

	cout << request;

	client->send(request);

	response = client->recv(4);
	if(response == "ERR\n")
		return false;

	response = client->recv(4);

	if(response != "OK\n" && response != "NEW\n") {
		return false;
	}

	return true;
}

void dirlist(TCPClient *client){
	string received;
	std::string token;
	std::vector<std::string> in;
	int i;

	client->send("LSD\n");
	client->recv(4);
	received = client->recvuntil("\n");


	std::istringstream sstrm(received);
	while(std::getline(sstrm, token, ' ')){
		in.push_back(token);
	}

	int n_dir = std::stoi(in[0], nullptr);
	if(n_dir == 0){
		return;
	}
	for(i = 1; i < (int)in.size() - 1; i++){
		std::cout << in[i] << std::endl;
	}

	std::cout << in[i];
}


void read_directory(const std::string& name, std::vector<std::string> *v){
    DIR* dir = opendir(name.c_str());
    struct dirent * dp;
    while ((dp = readdir(dir)) != NULL) {
        v->push_back(dp->d_name);
    }
    closedir(dir);
		return;
}
void backup(TCPClient *client){
	struct stat file;
	struct tm *t;
	char buffer[80];
	string in;
	std::string token, token2;
	std::vector<std::string> rec;
	string request;
	string received;
	cin >> in;
	std::vector<std::string> v;
	read_directory(in,&v);
	request = "BCK " + in + " " + to_string(v.size()) + "\n";
	client->send(request);
	for(int i = 0; i < (int)v.size(); i++){
		stat(v[i].c_str(), &file);
		t = gmtime(&(file.st_mtime));
	 	strftime (buffer,80,"%d.%m.%Y %X",t);
		request = v[i]+" ";
	  	request += buffer;
	  	request += " " + to_string(file.st_size) + "\n";
		client->send(request);
	}


	client->recv(4);
	received = client->recvuntil("\n");

	std::istringstream sstrm(received);
	while(std::getline(sstrm, token, ' ')){
		rec.push_back(token);
	}
	if(rec[0] == "ERR" || rec[0] == "EOF"){
		return;
	}
	std::vector< std::vector<std::string> > files(v.size());
	int i;
	for(i = 0; i < (int)v.size(); i++) {
		received = client->recvuntil("\n");
		std::istringstream sstrm(received);
		while(std::getline(sstrm, token, ' ')){
			files[i].push_back(token);
		}
	}
	for(auto a : files){
		for(std::string s : a){
			std::cout << s << " ";
		}
		std::cout << endl;
	}

}




int main(int argc, char const *argv[])
{	//tejo.tecnico.ulisboa.pt
	TCPClient* client = nullptr;
	bool logged_in = false;
	string addr = "localhost";
	int port = 58002;
	string prompt = "Input > ";
	string in;

	while(true) {
		cout << prompt;
		cin >> in;

		if (in == "exit") {
			cout << "Bye" << endl;
			if(client != nullptr) delete client;
			break;

		} else if (in == "login") {
			if(!logged_in) {
				std::string id;
				std::string pass;
				cin >> id >> pass;
				client = new TCPClient(AF_INET, port, addr);
				client->set_connection();

				if(login(client, id, pass) == true) {
					_id = id;
					_pass = pass;
					logged_in = true;
				}
				delete client;
				client = nullptr;
			}
		} else if (in == "dirlist") {
			if(!logged_in) {
				/* do nothing */
			} else {
				client = new TCPClient(AF_INET, port, addr);
				client->set_connection();
				if(login(client, _id, _pass)) {
					dirlist(client);
				} else {
					std::cout << "Couldn't authenticate" << std::endl;
				}

				delete client;
				client = nullptr;
			}

		}else if(in=="backup"){
			if(!logged_in) {
				/* do nothing */
			} else {
			client = new TCPClient(AF_INET, port, addr);
			client->set_connection();
			if(login(client, _id, _pass)) {
				backup(client);
			} else {
				std::cout << "Couldn't authenticate" << std::endl;
			}

			delete client;
			client = nullptr;
			}
		}else if (in == "logout") {
			if(logged_in) {
				logged_in = false;
			}
			else {
				std::cout << "Not logged in" << std::endl;
			}
		}

	}

	return 0;
}
