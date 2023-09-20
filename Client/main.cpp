#include <iostream>
#include <winsock2.h>

#pragma comment (lib,"ws2_32.lib")

int main() {
    // 初始化Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "初始化Winsock失败" << std::endl;
        return 1;
    }

    // 创建套接字
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "套接字创建失败" << std::endl;
        WSACleanup();
        return 1;
    }

    // 连接到服务器
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    //serverAddr.sin_addr.s_addr = inet_addr("服务器IP地址"); // 替换为服务器的IP地址
    serverAddr.sin_addr.s_addr = INADDR_ANY; // 替换为服务器的IP地址
    serverAddr.sin_port = htons(8888); // 端口号

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "连接服务器失败" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "连接服务器成功" << std::endl;

    // 发送和接收数据
    char message[] = "Hello, 服务器!";
    send(clientSocket, message, strlen(message), 0);

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead <= 0) {
        std::cerr << "连接关闭或接收数据失败" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "服务器回复: " << buffer << std::endl;

    // 关闭套接字和Winsock
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}