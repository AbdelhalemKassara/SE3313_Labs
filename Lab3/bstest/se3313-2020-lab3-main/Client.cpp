
#include "thread.h"
#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace Sync;

class ClientThread : Thread
{
private:
	Socket &sock;		 // create a socket, refer to Socket type
	bool &alive;		 // use this to determine if this thread is alive
	std::string msg; // input of user
	ByteArray d;		 // need a byte array to use with socket class

public:
	ClientThread(Socket &s, bool &alive) : sock(s), alive(alive) {}

	~ClientThread() {}

	virtual long ThreadMain()
	{
		// this is what will get executed when we create the thread in main method.
		while (true)
		{ // always loop
			try
			{
				std::cout << "Input your message: \n";
				std::cout.flush(); // request input and flush

				std::getline(std::cin, msg); // store the users input
				d = ByteArray(msg);					 // initialize d.. we will send this to sock.write()

				if (msg == "done")
				{ // if input is 'done' then break out of loop and close connection in main method
					alive = false;
					break;
				}
				// otherwise, write data to the server

				sock.Write(d);
				// check to make sure it was sent. Read will return an int of #bytes received.
				int conStatus = sock.Read(d);
				// if sock.Read returns 0, then there is no connection.
				if (conStatus == 0)
				{
					alive = false; // we should STOP -> break and close connection in main
					break;
				}
				else if (conStatus < 0)
				{
					// there is an error
					std::cout << "ERROR IN CLIENT, CONSTATUS < 0\n";
				}

				// if neither of those happen, successful data transfer to server. We want the response now.
				std::cout << "SERVER SAYS: '" << d.ToString() << "'\n";
			}
			catch (std::string error)
			{ // catch and send errors!
				std::cout << error << std::endl;
			}
		}
		return 0;
	}
};

int main(void)
{
	// Welcome the user
	std::cout << "SE3313 Lab 3 Client" << std::endl;
	bool alive = true;

	// Create our socket
	Socket socket("127.0.0.1", 3000);
	ClientThread cThread(socket, alive); // create a new client thread with the socket we specified

	socket.Open(); // OPEN THAT SOCKET BABY. Connect to the server.

	while (alive)
	{ // while the thread is 'alive' refresh every 1msec.. gets updated in ClientThread-> threadmain
		sleep(1);
	}

	socket.Close();

	return 0;
}
