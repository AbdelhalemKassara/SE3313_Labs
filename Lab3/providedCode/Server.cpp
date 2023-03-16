#include "thread.h"
#include "socketserver.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>
#include <stack>

#include <bits/stdc++.h>

using namespace Sync;

class ClientThread : public Thread
{
public:
    Socket *socket;
    bool *done = new bool(true);

    ClientThread(Socket *socket) : Thread()
    {
        *((*this).done) = false;
        (*this).socket = socket;
    }

    ~ClientThread()
    {
        std::cout << "deleted" << std::endl;
        (*socket).Close(); // close the socket
        delete socket;
        delete done;
    }

    virtual long ThreadMain(void)
    { // this needs to be in a while loop and only be closed when the client sends a signal or something for it close
        try
        {
            while (!(*done))
            {
                // std::cout << done << std::endl;

                ByteArray clientData;

                (*socket).Read(clientData);
                std::string clientString;
                clientString = clientData.ToString();

                if (clientString == "done")
                {
                    *done = true; // indicate that this thread has finished processing and ready to be deleted
                }
                else
                {

                    clientString = processString(clientString);
                    ByteArray out(clientString);
                    (*socket).Write(out);
                }
            }
        }
        catch (std::string error)
        {
            std::cout << error << std::endl;
        }
        catch (TerminationException e)
        {
            std::cout << "Server has been terminated" << std::endl;
        }

        return 0; // thread ends here
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
    try
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
    catch (...)
    {
        std::cout << "something when wrong when cleaning up the finished threads" << std::endl;
    }
}

int main(void)
{
    std::cout << "I am a server." << std::endl;
    SocketServer server(2000);
    std::vector<ClientThread *> threads;

    while (true)
    {
        try
        {
            Socket *socket = new Socket(server.Accept()); // this waits until a user connects
            threads.push_back(new ClientThread(socket));  // create a new thread to process the user request
            // std::cout << deleteFinishedThreads(&threads) << std::endl; // this delete the threads that are done
        }
        catch (std::string s)
        {
            std::cout << s << std::endl;
        }
    }
    // deleteFinishedThreads(&threads);
    server.Shutdown();
}
