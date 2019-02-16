FLAGS= -g -Wall -std=c++11
CC=g++

default: CS.cpp ../Server/TCPServer.cpp ../Server/UDPServer.cpp ../Client/UDPClient.cpp BServer.cpp User.cpp utils.cpp utils.hpp
	$(CC) $(FLAGS) ../Server/TCPServer.cpp ../Server/UDPServer.cpp ../Client/UDPClient.cpp BServer.cpp User.cpp utils.cpp CS.cpp  -o CS

run: CS
	./CS
