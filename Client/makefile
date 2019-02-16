FLAGS= -g -Wall -std=c++11
CC=g++

udp: Client.hpp UDPClient.hpp UDPClient.cpp udp_client.cpp
	$(CC) $(FLAGS) udp_client.cpp UDPClient.cpp -o udp_client 

tcp: Client.hpp TCPClient.hpp TCPClient.cpp udp_client.cpp
	$(CC) $(FLAGS) tcp_client.cpp TCPClient.cpp -o tcp_client 


run_udp: udp_client.cpp
	./udp_client

run_tcp: tcp_client.cpp
	./tcp_client