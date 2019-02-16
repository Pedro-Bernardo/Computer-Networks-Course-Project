#include "UDPClient.hpp"
#include <iostream>
// #include <signal.h>
#define MAXLINE 80

//lima

void clear_stdin() {
	char c;
	while((c = getchar()) != '\n');
}

int main(int argc, char* argv[]) {
	UDPClient client(AF_INET, 58000);
	char buffer[MAXLINE + 1];
	char ctrl = '\0';
	std::string prompt = "Press q to Quit, s to Send message: ";
	client.set_connection();

	
	while(ctrl != EOF) {
		std::cout << prompt; 
		scanf("%c", &ctrl);
		clear_stdin();

		if (ctrl == 'q') {
			std::cout << "Bye" << std::endl;
			break;
		}
		// clear stdin (TEMPORARY)
		printf("Input: ");
		if( (fgets(buffer, MAXLINE, stdin) == NULL) && ferror(stdin)) {
			perror("client error reading input");
		}
		buffer[strlen(buffer) - 1] = '\0';
		client.send(buffer);
		client.recv();
	}

	return 0;
}
