#ifndef SERVER_H
#define SERVER_H

#include "Socket.h"
#include <iostream>
#include <thread>
#include <vector>
#include <regex>
#include <cstdlib>
#include "ThreadSafeQueue.h"
#define MAXLINE 100

const char PAGE_404[] = "HTTP/1.1 404 Not Found\r\nContent-Type: \
text/html\r\nContent-Length: 38\r\n\r\n<HTML>\r\n<BODY>\r\n  404\
\r\n</BODY>\r\n</HTML>\r\n\0";

extern string fileBase; // = "D:/其他/NUS SWS/NUS Summer Workshop/Archer for website/Archer-Alpha";

using namespace std;

class Server
{
private:
    MySocket serverSocket;
    volatile bool running;

    // for multi-thread processing:
    SafeQueue<MySocket> connections;
    // static const int thread_num = 1;
    thread processorThread;

public:
    Server(unsigned short port);// I forget to add a seperate start() QAQ
    void start(unsigned short port);
    void addConnection(MySocket newSocket);
    void socketProcessor();
    void processSocket(int socketid);
    void stop();
    ~Server();
};

Server::Server(unsigned short port)
{
    running = true;
    thread ServerThread = thread(&Server::start, this, port); // start it in a thread
    ServerThread.detach();
}

void Server::start(unsigned short port)
{
    // this function was set as a thread with the aim of mulithread processing client sockets
    processorThread = thread(&Server::socketProcessor, this);

    serverSocket.bind(port); // bind to a port
    serverSocket.listen();

    while (running)
    {
        printf ("Server running");
        MySocket tmp(-1);
        //cout << "\nServer \n" << tmp.socketId <<" "<< socket.socketId << "\n";
        serverSocket.accept(tmp);
        //info("Server get one conn %d", tmp.socketId);
        addConnection(tmp);
    }
}

void Server::addConnection(MySocket newSocket)
{
    connections.enqueue(newSocket);
}

void Server::socketProcessor()
{
    //   the tid denote which of the the pendingConn[] to do
    // core loop here

    while (running)
    {
        //cout << "fuck1 dequeue";
        MySocket toResponse  = connections.dequeue();
        //cout << "dequeue a new socket "<<toResponse.socketId<<"\n";
        thread handlerThread = thread(&Server::processSocket, this, toResponse.socketId); // start a new thread to process it
        handlerThread.detach();
    }
    //once exit, close it's conn
}
void Server::processSocket(int socketid)
{
    //info(" process a new socket ");
    //revive the socket
    MySocket comming(socketid);

    while (running)
    {
        //read the buf
        char buf[MAXLINE + 1];
        comming.read(buf, MAXLINE);

        smatch smatch_head;
        regex regex_head("(.*) +(.*) +(HTTP\/[0-9][.][0-9])");
        string text(buf);

        if (!std::regex_search(text, smatch_head, regex_head))
        {
            //info("no header found, read again");
            break; // force the conn to close!!!
        }

        string opt_GET = smatch_head[1]; // GET
        string opt_filename = smatch_head[2]; // /index.html
        cout << "[info] operation: " << opt_GET << " " << opt_filename << endl;

        string contentType;
        smatch smatch_contentType;
        regex regex2("\\..*");
        if (!regex_search(opt_filename, smatch_contentType, regex2))
        {
            printf("no file extension found, use default!");
            contentType = "text/plain"; // if it does't match
        }
        else
        {
            if (smatch_contentType[0] == ".html")
            {
                contentType = "text/html";
            }
            else if (smatch_contentType[0] == ".xml")
            {
                contentType = "text/xml";
            }
            else if (smatch_contentType[0] == ".css")
            {
                contentType = "text/css";
            }
            else if (smatch_contentType[0] == ".png")
            {
                contentType = "image/png";
            }
            else if (smatch_contentType[0] == ".gif")
            {
                contentType = "image/gif";
            }
            else if (smatch_contentType[0] == ".jpg")
            {
                contentType = "image/jpg";
            }
            else if (smatch_contentType[0] == ".jpeg")
            {
                contentType = "image/jpeg";
            }
            else
            {
                contentType = "text/plain"; //TODO default not match!
            }
        }

        //std::cout <<"[debug] file at:"<< fileBase+opt_filename<<std::endl;
        std::ifstream t(fileBase + opt_filename, std::ios::binary);
        if (!t) //404 response!
        {
            // comming.socketStringStream << PAGE_404;
            const char *buff_404 = PAGE_404;
            comming.write(buff_404, sizeof(PAGE_404));
            printf("file not found!");
            break; // force the conn to close!!!
        }
        //reference
        //http://www.cplusplus.com/reference/fstream/ifstream/rdbuf/
        std::filebuf *tmp = t.rdbuf();
        int size = tmp->pubseekoff(0, t.end, t.in);
        tmp->pubseekpos(0, t.in);
        // new a buf to keep the dataS
        char *buffer = new char[size];
        // get data
        tmp->sgetn(buffer, size);
        t.close(); //close the file, but we've got the data!

        char rubbish[10] = "fuck";
        itoa(size, rubbish, 10);
        string socketString = "HTTP/1.1 200 OK\r\nServer: XJX\r\nContent-Length: "
                                + string(rubbish) +
                                "\r\nConnection: close\r\nContent-Type: "
                                + contentType + "\r\n\r\n";
        // comming.socketOutFlush();
        const char *buff_skt = socketString.c_str();
        int tempsize = strlen(buff_skt);
        comming.write(buff_skt, tempsize);

        comming.write(buffer, size);
        delete buffer; //clean up!
    }
    comming.close(); //close the conn!
}

void Server::stop()
{
    running = false;
    // close conn here !
}

Server::~Server()
{

}


#endif // SERVER_H
