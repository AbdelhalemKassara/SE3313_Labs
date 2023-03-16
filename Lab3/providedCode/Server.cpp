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
    std::vector<ClientThread *> *threads;
    ClientThread(Socket *socket, std::vector<ClientThread *> *threads) : Thread()
    {
        *((*this).done) = false;
        (*this).socket = socket;
        (*this).threads = threads;
    }

    ~ClientThread()
    {
        std::cout << "deleted" << std::endl;
    }

    virtual long ThreadMain(void)
    { // this needs to be in a while loop and only be closed when the client sends a signal or something for it close
        try
        {
            while (!(*done))
            {
                ByteArray clientData;

                int val = (*socket).Read(clientData);
                std::string clientString;
                clientString = clientData.ToString();

                if (clientString == "done" || clientString == "" || val <= 0)
                {
                    (*threads).erase(std::remove((*threads).begin(), (*threads).end(), this), (*threads).end()); // gets rid of this thread from the list
                    *done = true;                                                                                // indicate that this thread has finished processing and ready to be deleted
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
            return 0;
        }
        catch (std::string &error)
        {
            std::cout << error << std::endl;
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

class ReqThread : public Thread
{
private:
    SocketServer *server;
    std::vector<ClientThread *> threads;
    bool *endProgram;

public:
    ReqThread(SocketServer *server, bool endProgram)
    {
        (*this).server = server;
        (*this).endProgram = &endProgram;
    }
    ~ReqThread()
    {
        try
        {
            for (int i = 0; i < threads.size(); i++)
            {
                (*(*threads[i]).socket).Close(); // close all the open sockets
            }
        }
        catch (...)
        {
            std::cout << "There was an issue with cleaning up the files" << std::endl; // catch errors
        }
        *endProgram = true;
    }
    virtual long ThreadMain(void)
    {
        while (!*endProgram)
        {
            try
            {
                if (*endProgram)
                {
                    break;
                }
                Socket temp = (*server).Accept();

                Socket *socket = new Socket(temp);                     // this waits until a user connects
                threads.push_back(new ClientThread(socket, &threads)); // create a new thread to process the user request
            }
            catch (std::string s)
            {
                // std::cout << s << std::endl;
            }
            catch (TerminationException e)
            {
                std::cout << "Server has been terminated." << std::endl;
            }
        }
        return 0;
    }
};
int main(void)
{
    std::cout << "I am a server." << std::endl;
    SocketServer server(2000);
    bool endProgram = false;
    ReqThread reqThread(&server, endProgram);

    FlexWait cinWaiter(1, stdin);
    cinWaiter.Wait();
    std::cin.get();

    endProgram = true;
    server.Shutdown();

    std::cout << "Server has shutdown" << std::endl;
}
