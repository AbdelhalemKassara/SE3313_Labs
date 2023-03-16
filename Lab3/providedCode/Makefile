all: Client Server

Client : Client.o socket.o Blockable.o thread.o
	g++ -g -o Client.out Client.o thread.o socket.o Blockable.o -pthread -l rt

Client.o : Client.cpp socket.h
	g++ -g -c Client.cpp -std=c++11

Server : Server.o thread.o socket.o socketserver.o Blockable.o
	g++ -g -o Server.out Server.o thread.o socket.o socketserver.o Blockable.o -pthread -l rt

Blockable.o : Blockable.h Blockable.cpp
	g++ -g -c Blockable.cpp -std=c++11

Server.o : Server.cpp thread.h socketserver.h
	g++ -g -c Server.cpp -std=c++11

thread.o : thread.cpp thread.h
	g++ -g -c thread.cpp -std=c++11

socket.o : socket.cpp socket.h
	g++ -g -c socket.cpp -std=c++11

socketserver.o : socketserver.cpp socket.h socketserver.h
	g++ -g -c socketserver.cpp -std=c++11
