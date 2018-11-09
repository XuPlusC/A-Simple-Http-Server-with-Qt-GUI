#ifndef SOCKET_H
#define SOCKET_H

#pragma comment(lib, "WS2_32.lib") //?? add it here?

#include <string>
#include <fstream>
#include <sstream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#define MAJORVERSION 2 //Winsock主版本号
#define MINORVERSION 2 //Winsock次版本号，应该为0？

using namespace std;

class MySocket
{
public:
    int port;
    int socketId;

    MySocket();
    MySocket(int socketId);
    MySocket operator = (MySocket &socket);
    MySocket(const MySocket &socket);
    ~MySocket();

    void connect(const string &ipAddr, unsigned short port);
    void bind(unsigned short port);
    void listen();
    void accept(MySocket& comming);
    void close();

    // winsock startup
    int WinsockEnvStartup();

    //socket IO
    int read(char *recvbuf, int n);
    int write(const char *usrbuf, int n);

    //stream IO
    // void socketOutFlush();
};

MySocket::MySocket()
{
    //初始化 winsock
    int rtn = WinsockEnvStartup();

    if (rtn == -1)
    { //如果返回值不是0，则Winsock初始化失败
        cout << "Winsock startup error!\n";
        return;	// what should i do here?
    }
    cout << "socket constricted!" << endl;
    socketId = socket(AF_INET, SOCK_STREAM, 0);
    if (socketId < 0)
        cout << "construct socket error!" << endl;
}

MySocket::MySocket (const MySocket &fk)
{
    this->socketId = fk.socketId;
}

MySocket MySocket::operator = (MySocket &socket)
{
    this->socketId = socket.socketId;
    return *this;
}

int MySocket::WinsockEnvStartup()
{
    WSADATA wsaData;											   //用于返回Winsock的环境信息
    int rtn;													   //Winsock初始化函数的返回值
    WORD wVersionRequested = MAKEWORD(MAJORVERSION, MINORVERSION); //Winsock版本号，高字节次版本，低字节主版本号

    //初始化 winsock
    rtn = WSAStartup(wVersionRequested, &wsaData);

    if (rtn)
    { //如果返回值不是0，则Winsock初始化失败
        cout << "Winsock startup error!\n";
        return -1;
    }

    //判断返回的Winsock版本号
    if (LOBYTE(wsaData.wVersion) != MAJORVERSION || HIBYTE(wsaData.wVersion) != MINORVERSION)
    {				  //如果版本不对
        WSACleanup(); //清除Winsock
        cout << "Winsock version error!\n";
        return -1;
    }

    cout << "Winsock startup ok!\n";
    return 0;
}

MySocket::MySocket(int Id)
{
    socketId = Id;
}

MySocket::~MySocket()
{
    //WSACleanup();
    //you must destroy socket maually
    //info( "delete socket");
}

void MySocket::bind(unsigned short port)
{
    struct sockaddr_in servAddr;
    char optval = 1;
    memset(&servAddr, 0, sizeof(servAddr));

    //设置服务器IP地址和端口号
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

    //绑定 socket to Server's IP and port
    int rtn; /* = setsockopt(socketId, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(char));
    if (rtn < 0)
        cout << "setsockopt failed!" << endl; */
    rtn = ::bind(socketId, (struct sockaddr *)&servAddr, sizeof(servAddr));
    if (rtn < 0)
        cout << "bind failed!" << WSAGetLastError() << endl;
}

void MySocket::listen()
{
    int err = ::listen(socketId, SOMAXCONN);
    if (err < 0)
        cout << "Listen failed! " <<socketId<<" "<<err<< endl;
}

void MySocket::accept(MySocket& comming)
{
    SOCKET sk = -1;

    struct sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));
    socklen_t socklen = sizeof(cliaddr);
    sk = ::accept(socketId, (struct sockaddr *)&cliaddr, &socklen);
//#pragma warning(disable : 4996)
    printf("[info] get a socket from %s\n", inet_ntoa(cliaddr.sin_addr));
    if (sk <= 0)
        cout << "accept failed! ";

    comming.socketId = sk;
}

void MySocket::close()
{
    int err = closesocket(socketId);
    if (err < 0)
        cout << "close error!" << endl;
}

int MySocket::read(char *recvbuf, int n) {
    // if (socketId < 0)
    // 	cout << "read error !" << endl;
    int left = n;
    char *bufp = recvbuf;
    int read = 0;

    read = ::recv(socketId, recvbuf, left, 0);
    //if (read == 0) break;//end
    // if (read > 0)
        // cout << "read failed! force thread to terminate!" << endl;
    if (read < 0)
        std::terminate();
    left -= read;
    bufp += read;
    *bufp = '\0';
    return n - left;//left to read?
};

int MySocket::write(const char *usrbuf, int n) {
    // fputs(usrbuf, stdout);
    // if (socketId >= 0)
    // cout << "write error !" << endl;
    int left = n;
    const char *bufp = usrbuf;
    int written = 0;

    while (left > 0)
    {
        written = ::send(socketId, usrbuf, left, 0);
        if (written == 0)
            break;//end
        if (written < 0)
            std::terminate();
        left -= written;
        bufp += written;
    }
    // cout << "left to written " << left << endl;
    return n - left;
};

#endif
