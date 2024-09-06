// ChatAppUtils.h
#pragma once
#include <winsock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

//initialize Winsock
bool initWinsock() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed. Error Code: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

//cleanup Winsock
void cleanupWinsock() {
    WSACleanup();
}

//create a socket
SOCKET createSocket() {
    SOCKET socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed. Error Code: " << WSAGetLastError() << std::endl;
        cleanupWinsock();
    }
    return socket_fd;
}

//send a message to a socket
void sendMessage(SOCKET socket_fd, const std::string& message) {
    send(socket_fd, message.c_str(), message.size(), 0);
}

//receive a message from a socket
std::string receiveMessage(SOCKET socket_fd) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int bytes_received = recv(socket_fd, buffer, sizeof(buffer), 0);
    if (bytes_received > 0) {
        return std::string(buffer, bytes_received);
    }
    return "";
}

//close the socket
void closeSocket(SOCKET socket_fd) {
    closesocket(socket_fd);
}