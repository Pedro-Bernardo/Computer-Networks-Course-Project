#define PORT 58000
//lima

#include "UDPServer.hpp"

void handler(Server* server) {

}

int main(int argc, char* argv[]) {

	UDPServer server(AF_INET, 58000);

	server.setup();
	server.serve(handler);

	return 0;
}
