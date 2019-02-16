#define PORT 58000
//lima

#include "TCPServer.hpp"

void handler(Server* server) {
	int n = 0;
	while((n = server->recv()) != -1) {
		printf("server received: %s\n", ((TCPServer*)server)->get_msg());
		server->send("OK\0");
	}
}

int main(int argc, char* argv[]) {

	TCPServer server(AF_INET, 58070);

	server.setup();
	server.serve(handler);

	return 0;
}
