#include <iostream>
#include <winsock2.h>

#pragma comment (lib,"ws2_32.lib")

int main() {
    // ��ʼ��Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "��ʼ��Winsockʧ��" << std::endl;
        return 1;
    }

    // �����׽���
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "�׽��ִ���ʧ��" << std::endl;
        WSACleanup();
        return 1;
    }

    // ���ӵ�������
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    //serverAddr.sin_addr.s_addr = inet_addr("������IP��ַ"); // �滻Ϊ��������IP��ַ
    serverAddr.sin_addr.s_addr = INADDR_ANY; // �滻Ϊ��������IP��ַ
    serverAddr.sin_port = htons(8888); // �˿ں�

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "���ӷ�����ʧ��" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "���ӷ������ɹ�" << std::endl;

    // ���ͺͽ�������
    char message[] = "Hello, ������!";
    send(clientSocket, message, strlen(message), 0);

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead <= 0) {
        std::cerr << "���ӹرջ��������ʧ��" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "�������ظ�: " << buffer << std::endl;

    // �ر��׽��ֺ�Winsock
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}