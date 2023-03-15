#include "thread.h"
#include "socketserver.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>
#include <stack>

#include <bits/stdc++.h>
#include <thread>
// #include <socket.h>

using namespace Sync;

class ClientThread : public Thread
{
    Socket &socket;

public:
    ClientThread(Socket &s) : socket(s)
    {
    }

    virtual long ThreadMain(void) override
    {
        try
        {
            ByteArray clientData;

            socket.Read(clientData);
            std::string clientString;
            clientString = clientData.ToString();

            std::cout << clientString << std::endl;

            clientString = processString(clientString);
            ByteArray out(clientString);
            socket.Write(out);
        }
        catch (std::string error)
        {
            std::cout << error << std::endl;
        }

        return 1; // thread ends here
    }

    // converts the input string to all upper case
    std::string processString(std::string s)
    {
        transform(s.begin(), s.end(), s.begin(), ::toupper);
        return s;
    }
};

class ServerThread : public Thread
{
};

int main(void)
{
    // std::stack<ClientThread *> threadsStack; // we will store the threads in this stack

    std::cout << "I am a server." << std::endl;
    SocketServer server(2000);
    ClientThread *thread;

    while (true)
    {
        std::cout << "loop" << std::endl;
        Socket *socket = new Socket(server.Accept()); // this waits until a user connects
        thread = new ClientThread(*socket);
    }
}
