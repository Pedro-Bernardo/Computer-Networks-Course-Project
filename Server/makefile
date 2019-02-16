FLAGS= -g -Wall -std=c++11
CC=g++

udp: Server.hpp UDPServer.hpp UDPServer.cpp udp_server.cpp
	$(CC) $(FLAGS) udp_server.cpp UDPServer.cpp -o udp_server 

tcp: Server.hpp tcpServer.hpp tcpServer.cpp tcp_server.cpp
	$(CC) $(FLAGS) tcp_server.cpp TCPServer.cpp -o tcp_server

run_udp: udp_server.cpp
	./udp_server

run_tcp: tcp_server.cpp
	./tcp_server