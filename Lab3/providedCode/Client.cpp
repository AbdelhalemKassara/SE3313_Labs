#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace Sync;

std::string userDataStr;

int main(void)
{
	std::cout << "I am a client" << std::endl;

	while (true)
	{

		std::cout << "Enter some Text: ";

		std::getline(std::cin, userDataStr); // takes in the whole string including spaces
		if (userDataStr == "done")
		{
			break;
		}
		Socket socket("127.0.0.1", 2000);
		socket.Open();

		ByteArray byteData(userDataStr);

		socket.Write(byteData); // this waits until it gets data from the server

		ByteArray returnVal;
		int worked = socket.Read(returnVal);

		if (worked > 0) // everything is fine
		{
			std::string print = returnVal.ToString();
			std::cout << "Response: " << print << std::endl
								<< std::endl;
		}
		else if (worked == 0) // the connection on the server closed
		{
		}
		else // there was an issue
		{
		}
	}

	return 1;
}
