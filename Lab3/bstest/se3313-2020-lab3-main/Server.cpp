#include "thread.h"
#include "socketserver.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>
#include <algorithm>

using namespace Sync;

// handle clients sockets
class SocketThread : public Thread
{
private:
	Socket &sock;
	ByteArray d;
	bool &end;
	std::vector<SocketThread *> &sThreadList;

public:
	SocketThread(Socket &s, bool &end, std::vector<SocketThread *> &clientSocketThread) : sock(s), end(end), sThreadList(clientSocketThread) {}

	~SocketThread()
	{
		this->terminationEvent.Wait();
	}
	Socket &getSocket()
	{
		return sock;
	}

	virtual long ThreadMain()
	{

		try
		{
			while (!end)
			{
				sock.Read(d);

				// do something to the data.
				std::string newMsg = d.ToString();

				if (newMsg == "done")
				{																																																	 // if message is done then we are gonna get rid of all the threads
					sThreadList.erase(std::remove(sThreadList.begin(), sThreadList.end(), this), sThreadList.end()); // get rid of all threads.
					end = true;
					break;
				}

				std::for_each(newMsg.begin(), newMsg.end(), [](char &newMsg)
											{ newMsg = ::toupper(newMsg); });

				newMsg.append("-- your updated message IS LOUD");
				sock.Write(ByteArray(newMsg)); // send data to client
			}
		}
		catch (std::string &s)
		{
			std::cout << s << std::endl; // catch errors
		}
		catch (std::string error)
		{
			std::cout << error << std::endl;
		}
		std::cout << "Client has disconnected" << std::endl;
		return 0;
	}
};

// This thread handles the server operations
class ServerThread : public Thread
{
private:
	SocketServer &server;
	bool end = false;
	std::vector<SocketThread *> sThreadList;

public:
	ServerThread(SocketServer &server)
			: server(server)
	{
	}

	~ServerThread()
	{
		// Cleanup
		for (auto t : sThreadList)
		{
			try
			{
				Socket &s = t->getSocket();
				s.Close();
			}
			catch (...)
			{
				std::cout << "Something went wrong in server cleanup" << std::endl;
			}
		}
		std::vector<SocketThread *>().swap(sThreadList); // swaps the lists.. now our list is empty (swapped w an empty list)
		std::cout << "Stopping the Client FROM the Server." << std::endl;
		end = true;
	}

	virtual long ThreadMain()
	{
		while (1)
		{
			try
			{
				// Wait for a client socket connection
				Socket *newConnection = new Socket(server.Accept());

				ThreadSem serverBlocker(1);

				// A reference to this pointer
				Socket &socketReference = *newConnection;

				sThreadList.push_back(new SocketThread(socketReference, end, std::ref(sThreadList)));
				// add a new SocketThread to our list
			}
			catch (std::string error)
			{
				std::cout << "ERROR IN ServerThread THREADMAIN :" << error << ".\n";
				return 1;
			}
			catch (TerminationException t)
			{
				std::cout << "Server is terminated! Goodbye!!" << std::endl;
				return t;
			}
		}

		return 0;
	}
};

int main(void)
{
	std::cout << "I am a server." << std::endl;
	std::cout << "Press any key to terminate server..." << std::endl;
	std::cout.flush();

	// Create our server
	SocketServer server(3000);

	// Need a thread to perform server operations
	ServerThread serverThread(server);

	// This will wait for input to shutdown the server
	FlexWait cinWaiter(1, stdin);
	cinWaiter.Wait();
	std::cin.get();

	// Shut down and clean up the server
	server.Shutdown();
}
