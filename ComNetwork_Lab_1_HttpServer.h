#ifndef OLDMAIN_H
#define OLDMAIN_H

#include "Server.hpp"
#include <cstdio>
#include <iostream>
#include <fstream>

using namespace std;

extern string Path;

string fileBase;// = "D:/其他/NUS SWS/NUS Summer Workshop/Archer for website/Archer-Alpha";

int StartServer(int Port)
{
    //start thread pool
    // HandlerPool handlerPool;
    //handlerPool.start();
    //get a port from conf

    unsigned short port = Port;
    fileBase = Path;
    cout << Path << endl;
    //read from input

    //cin >> fileBase;

    //start listener thread
    Server server(port);

    //wait for input to close
    //getchar();
    //server.stop();
    cout << "server start!" <<endl;
    return 0;
}
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示:
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

#endif
