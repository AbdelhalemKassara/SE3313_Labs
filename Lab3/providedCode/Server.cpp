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
public:
    Socket *socket;
    bool *done = new bool(false);

    ClientThread(Socket *socket)
    {
        (*this).socket = socket;
    }
    ~ClientThread() {}

    virtual long ThreadMain(void) override
    {
        try
        {
            ByteArray clientData;

            (*socket).Read(clientData);
            std::string clientString;
            clientString = clientData.ToString();

            std::cout << clientString << std::endl;

            clientString = processString(clientString);
            ByteArray out(clientString);
            (*socket).Write(out);
        }
        catch (std::string error)
        {
            std::cout << error << std::endl;
        }
        catch (std::string &error)
        {
            std::cout << error << std::endl;
        }

        *done = true;
        return 1; // thread ends here
    }

    // converts the input string to all upper case
    std::string processString(std::string s)
    {
        transform(s.begin(), s.end(), s.begin(), ::toupper);
        return s;
    }
};

// check if any threads have completed then delete them
int deleteFinishedThreads(std::vector<ClientThread *> *threads)
{
    for (int i = 0; i < (*threads).size(); i++)
    {
        if (*((*(*threads)[i]).done) == 1)
        {
            delete (*threads)[i];
            (*threads).erase((*threads).begin() - i);
        }
    }

    return (*threads).size();
}

int main(void)
{
    std::cout << "I am a server." << std::endl;
    SocketServer server(2000);
    std::vector<ClientThread *> threads;

    while (true)
    {
        Socket *socket = new Socket(server.Accept()); // this waits until a user connects

        threads.push_back(new ClientThread(socket)); // create a new thread to process the user request

        // deleteFinishedThreads(&threads);
    }
}
