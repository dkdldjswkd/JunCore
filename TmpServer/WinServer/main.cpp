#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 7777
#define BUFFER_SIZE 1024

void initializeWinsock() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        exit(1);
    }
}

void cleanupWinsock() {
    WSACleanup();
}

int main() {
    initializeWinsock();

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create server socket." << std::endl;
        cleanupWinsock();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind server socket." << std::endl;
        closesocket(serverSocket);
        cleanupWinsock();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Failed to listen on server socket." << std::endl;
        closesocket(serverSocket);
        cleanupWinsock();
        return 1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    SOCKET clientSocket;

    for (;;)
    {
        clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Failed to accept client connection." << std::endl;
            closesocket(serverSocket);
            cleanupWinsock();
            return 1;
        }

        char buffer[BUFFER_SIZE];
        while (true) {
            int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
            if (bytesReceived > 0) {
                buffer[bytesReceived] = '\0';  // Null-terminate the received string
                std::cout << "Received from client: " << buffer << std::endl;

                send(clientSocket, buffer, bytesReceived, 0);  // Echo back to client

                if (0 == std::strcmp(buffer, "close"))
                    goto terminated_ping_pong;
            }
            else if (bytesReceived == 0) {
                std::cout << "Client disconnected." << std::endl;
                break;
            }
            else {
                int errorCode = WSAGetLastError();
                if (WSAECONNRESET == errorCode || WSAECONNABORTED == errorCode)
                {
                    std::cerr << "client is disconnected" << std::endl;
                }
                else
                {
                    std::cerr << "Error code : " << errorCode << std::endl;
                }
                break;
            }
        }
    }

terminated_ping_pong:


    closesocket(clientSocket);
    closesocket(serverSocket);
    cleanupWinsock();

    std::cout << "server terminated" << std::endl;
    return 0;
}
