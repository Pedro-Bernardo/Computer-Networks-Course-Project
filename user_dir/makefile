FLAGS= -g -Wall -std=c++11
CC=g++

default: user.cpp ../Client/TCPClient.cpp 
	$(CC) $(FLAGS) ../Client/TCPClient.cpp user.cpp  -o user 

run: user
	./user