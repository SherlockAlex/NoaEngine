#include <cstring>
#include <iostream>
#include <winsock2.h>

int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
    {
        std::cerr << "初始化" << std::endl;
    }
}

