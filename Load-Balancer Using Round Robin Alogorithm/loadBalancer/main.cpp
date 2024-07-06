#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

#define PORT 8083
#define BUFFER_SIZE 1024

std::vector<std::pair<std::string, int>> servers = {
    {"127.0.0.1", 8080},
    {"127.0.0.1", 8081},
    {"127.0.0.1", 8082}
};
int serverIndex = 0;

SOCKET connectToServer() {
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(servers[serverIndex].second);
    inet_pton(AF_INET, servers[serverIndex].first.c_str(), &serverAddr.sin_addr);

    if (connect(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection to server failed" << std::endl;
        return INVALID_SOCKET;
    }

    serverIndex = (serverIndex + 1) % servers.size();
    return serverSocket;
}

void handleClient(SOCKET clientSocket) {
    char buffer[BUFFER_SIZE];
    SOCKET serverSocket = connectToServer();
    if (serverSocket == INVALID_SOCKET) {
        closesocket(clientSocket);
        return;
    }

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesRead <= 0) {
            break;
        }
        send(serverSocket, buffer, bytesRead, 0);
        memset(buffer, 0, BUFFER_SIZE);
        bytesRead = recv(serverSocket, buffer, BUFFER_SIZE, 0);
        if (bytesRead <= 0) {
            break;
        }
        send(clientSocket, buffer, bytesRead, 0);
    }

    closesocket(serverSocket);
    closesocket(clientSocket);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    SOCKET loadBalancerSocket, clientSocket;
    struct sockaddr_in loadBalancerAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    loadBalancerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (loadBalancerSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    loadBalancerAddr.sin_family = AF_INET;
    loadBalancerAddr.sin_addr.s_addr = INADDR_ANY;
    loadBalancerAddr.sin_port = htons(PORT);

    if (bind(loadBalancerSocket, (struct sockaddr*)&loadBalancerAddr, sizeof(loadBalancerAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed" << std::endl;
        return 1;
    }

    if (listen(loadBalancerSocket, 6) == SOCKET_ERROR) {
        std::cerr << "Listen failed" << std::endl;
        return 1;
    }

    std::cout << "Load balancer listening on port " << PORT << std::endl;

    while ((clientSocket = accept(loadBalancerSocket, (struct sockaddr*)&clientAddr, &clientAddrLen)) != INVALID_SOCKET) {
        std::cout << "Client connected to load balancer" << std::endl;
        std::thread(handleClient, clientSocket).detach();
    }

    closesocket(loadBalancerSocket);
    WSACleanup();
    return 0;
}
