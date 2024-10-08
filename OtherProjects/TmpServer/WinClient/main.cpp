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

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "Failed to create client socket." << std::endl;
		cleanupWinsock();
		return 1;
	}

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = htons(PORT);

	if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cerr << "Failed to connect to server." << std::endl;
		closesocket(clientSocket);
		cleanupWinsock();
		return 1;
	}

	std::string message;
	char buffer[BUFFER_SIZE];
	while (true) {
		std::cout << ">> ";
		std::getline(std::cin, message);

		if (message.empty()) continue;

		send(clientSocket, message.c_str(), message.length(), 0);

		if(message == "close")
			goto terminated_ping_pong;

		int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
		if (bytesReceived > 0) {
			buffer[bytesReceived] = '\0';  // Null-terminate the received string
			std::cout << "Server: " << buffer << std::endl;
		}
		else if (bytesReceived == 0) {
			std::cout << "Server disconnected." << std::endl;
			break;
		}
		else {
			std::cerr << "Error in receiving data." << std::endl;
			break;
		}
	}

terminated_ping_pong:

	closesocket(clientSocket);
	cleanupWinsock();

	std::cout << "client terminated" << std::endl;
	return 0;
}