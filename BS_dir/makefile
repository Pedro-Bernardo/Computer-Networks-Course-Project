FLAGS= -g -Wall -std=c++11
CC=g++

default: BS.cpp ../Client/UDPClient.cpp ../Server/UDPServer.cpp Dir.hpp File.hpp User.cpp User.hpp
	$(CC) $(FLAGS) ../Client/UDPClient.cpp ../Server/UDPServer.cpp User.cpp BS.cpp -o BS

run: BS
	./BS
