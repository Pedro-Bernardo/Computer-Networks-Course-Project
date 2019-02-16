
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

extern std::list<BServer*>  servers;
extern char server_db[];
extern User* user;
extern bool connected;

void add_bserver(BServer* server) {
	std::ofstream ofs;
	ofs.open(server_db, std::ofstream::app);
	/* write a "-" to signal the start of a server */
	ofs << "- " << *server << " |\n";
	ofs.close();
}

void update_bserver_db() {
	std::ofstream ofs;
	ofs.open(server_db, std::ofstream::out);
	/* write a "-" to signal the start of a server */
	ofs << "BEGIN |\n";
	for(BServer* server : servers) {
		ofs << "- " << *server << " |\n";
	}

	ofs.close();
}

BServer* get_bserver(const std::string& ip, int port) {
    for(BServer* el : servers) {
        std::cout << el->get_ip() << " | " << el->get_port() << std::endl;
        if(el->get_ip() == ip && el->get_port() == port) {
            std::cout << "RETURNING EL" << std::endl;
            return el;
        }

    }

    return nullptr;
}

void load_bservers() {
	std::ifstream ifs;
	ifs.open(server_db, std::ofstream::in);
	std::string flag;
	/* free every server and map */
	for(BServer* srv : servers) {
		delete srv;
	}
	servers.clear();
	// the pointers are the same, and they are already deleted (freed)

	ifs >> flag >> std::ws;
	if( flag == "BEGIN" ) {
		ifs >> flag;
		while(flag[0] == '|') {
			std::string ctrl;
			ifs >> ctrl >> std::ws;
			if (ctrl != "-")  /* check if there is a server to read */
				return;

			BServer* srv = new BServer();
			ifs >> *srv;

			servers.push_front(srv);
			ifs >> flag >> std::ws;
		}
		/* sort the servers by count in descending order */
		std::cout << "SORTING" << std::endl;
		servers.sort(compare_count);

	} else {
		std::cout << server_db << " file is corrupt!" << std::endl;
		return;
	}
}


bool compare_count(const BServer* first, const BServer* second){
  return (first->get_count() > second->get_count());
}

std::ostream& operator<< ( std::ostream& stream, const BServer& server ){
	std::string aux = (server.is_available()) ? "true" : "false";
	stream << aux<<' '<<server.get_ip() << ' ' << server.get_port() << ' ' << server.get_count();

	return stream;
}

std::istream& operator>> ( std::istream& stream, BServer& server ) {
	std::string in;
	stream >> in >> std::ws;

	if(in == "true") {
		server._available=true;
	} else if (in == "false") {
		server._available=false;
	}
	stream >>server._ip >> std::ws >> server._port >> std::ws >> server._count;

	return stream;
}

std::ostream& operator<< ( std::ostream& stream, const User& usr ) {
	std::map< std::string, BServer* > backups = usr.get_backups();
	std::string logged_in = (usr.is_logged_in()) ? "true" : "false";

	std::cout << logged_in << std::endl;

	stream <<  usr.get_id() << '\n'
		   << usr.get_passwd() << '\n'
		   << logged_in << '\n';

	for(auto pair : backups) {
		stream << pair.first << ' ' << ((BServer*)(pair.second))->get_ip() << ' ' << ((BServer*)(pair.second))->get_port() << '\n';
	}

	stream << "END";
	return stream;
}

std::istream& operator>> ( std::istream& stream, User& usr ) {
	int id;
	std::string passwd;
	std::string in;
	stream >> id >> std::ws; /* read id and discard whitespace */

	std::getline(stream, passwd);

	user = new User(id, passwd);

	stream >> in >> std::ws;

	if(in == "true") {
		user->login();
	} else if (in == "false") {
		user->logout();
	}

	while(true) {
        BServer* srv;
		std::string dir;
        std::string ip;
        int port;
		/* read dirname */
		stream >> in;
		if(in == "END") break;
		dir = in;
		/* clear space */
		stream >> std::ws;
		/* read server ip and clear newline */
		stream >> ip >> std::ws;
        /* read the port */
        stream >> port >> std::ws;
        srv = get_bserver(ip, port);

        if(srv == nullptr) {
            delete user;
            user = nullptr;
            std::cout << "Error reading user" << std::endl;
            return stream;
        } else {
            user->add_backup(dir, srv);
        }

	}

	return stream;
}

void write_user() {
	User to_write = *user;
	std::cout << "Writing to file " << std::endl;
	std::string fname = std::to_string(user->get_id());
	std::ofstream(fname) << to_write;
}

void read_user(const std::string& fname) {
	std::cout << "Reading from file " << std::endl;
    std::ifstream(fname) >> *user;
}
