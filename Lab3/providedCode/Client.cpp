#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "thread.h"

using namespace Sync;

class ClientThread : Thread
{
private:
	Socket *socket;
	bool *done;

public:
	ClientThread(Socket *socket, bool *done)
	{
		(*this).socket = socket;
		(*this).done = done;
	}
	~ClientThread() {}

	virtual long ThreadMain()
	{
		while (true)
		{
			try
			{
				// get the user input
				std::string userInputStr;
				std::cout << "Enter some Text: ";
				std::getline(std::cin, userInputStr);

				ByteArray bArr(userInputStr);
				(*socket).Write(bArr); // write data to the server

				// if the user is done break out of the loop
				if (userInputStr == "done")
				{
					break;
				}

				int conStatus = (*socket).Read(bArr); // waits until we get data back from the server

				if (conStatus == 0) // There is no connection with the server
				{
					break;
				}
				else if (conStatus < 0) // there was an issue with the request, server side
				{
					std::cout << "There was an issue processing your request, connection status < 0" << std::endl;
				}
				else // prints out the processed string returned by the server
				{
					std::cout << "Response: " << bArr.ToString() << std::endl;
				}
			}
			catch (std::string s)
			{
				std::cout << s << std::endl;
			}
		}
		*done = true;

		return 0;
	}
};

int main(void)
{
	std::cout << "I am a client" << std::endl;
	bool done = false;
	Socket socket("127.0.0.1", 2000);

	ClientThread thread(&socket, &done); // passes in the address of done and socket

	socket.Open();

	while (!done)
	{
		sleep(1); // while the program is not done wait 2ms (to not waste resources)
	}

	socket.Close();

	return 1;
}
