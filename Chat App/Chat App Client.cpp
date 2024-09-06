// ChatAppClient.cpp
#include "ChatAppUtils.h"
#include <thread>

void receiveMessages(SOCKET client_socket) {
    while (true) {
        std::string message = receiveMessage(client_socket);
        if (!message.empty()) {
            std::cout << "Message received: " << message << std::endl;
        }
    }
}

int main() {
    //initialize Winsock
    if (!initWinsock()) return -1;

    //create client socket
    SOCKET client_socket = createSocket();
    if (client_socket == INVALID_SOCKET) return -1;

    //configure server address
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    //connect to server
    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed. Error Code: " << WSAGetLastError() << std::endl;
        closeSocket(client_socket);
        cleanupWinsock();
        return -1;
    }

    //start thread to receive messages
    std::thread(receiveMessages, client_socket).detach();

    //send messages to server
    std::string message;
    while (true) {
        std::getline(std::cin, message);
        sendMessage(client_socket, message);
    }

    //cleanup
    closeSocket(client_socket);
    cleanupWinsock();
    return 0;
}