#include <cstring>
#include <iostream>
#include <winsock2.h>

#pragma comment (lib,"ws2_32.lib")

int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2),&wsaData)!=0)
    {
        std::cerr << "初始化失败" << std::endl;
        return 1;
    }
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cerr << "创建套接字失败" << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8888);

    if (bind(serverSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr))==SOCKET_ERROR)
    {
        std::cerr << "绑定失败" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket,5) == SOCKET_ERROR)
    {
        std::cerr << "监听失败" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "等待客户端连接..." << std::endl;

    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "接受客户端连接失败" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "客户端连接成功" << std::endl;

    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int byteRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (byteRead <=0)
        {
            std::cerr << "连接关闭或者接受数据失败" << std::endl;
            break;
        }

        std::cout << "客户端：" << buffer << std::endl;
        const char* response = "服务端收到消息";
        send(clientSocket,response,strlen(response),0);

    }

}

