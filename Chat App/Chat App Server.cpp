// ChatAppServer.cpp
#include "ChatAppUtils.h"
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>

std::vector<SOCKET> clients;
std::mutex clients_mutex;

//broadcast a message to all clients
void broadcastMessage(const std::string& message, SOCKET sender) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (SOCKET client : clients) {
        if (client != sender) {
            sendMessage(client, message);
        }
    }
}

//handle communication with a client
void handleClient(SOCKET client_socket) {
    while (true) {
        std::string message = receiveMessage(client_socket);
        if (message.empty()) {
            // Client disconnected or an error occurred
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
            closeSocket(client_socket);
            break;
        }
        broadcastMessage(message, client_socket);
    }
}

int main() {
    //initialize Winsock
    if (!initWinsock()) return -1;

    //create server socket
    SOCKET server_socket = createSocket();
    if (server_socket == INVALID_SOCKET) return -1;

    //configure server address
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //bind the socket
    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Binding failed. Error Code: " << WSAGetLastError() << std::endl;
        closeSocket(server_socket);
        cleanupWinsock();
        return -1;
    }

    //listen for incoming connections
    if (listen(server_socket, 5) == SOCKET_ERROR) {
        std::cerr << "Listening failed. Error Code: " << WSAGetLastError() << std::endl;
        closeSocket(server_socket);
        cleanupWinsock();
        return -1;
    }

    std::cout << "Server started on port 8080\n";

    //accept and handle clients
    while (true) {
        SOCKET client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed. Error Code: " << WSAGetLastError() << std::endl;
            continue;
        }

        //add the client to the list
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(client_socket);
        }

        //start a new thread to handle the client
        std::thread(handleClient, client_socket).detach();
    }

    //cleanup
    closeSocket(server_socket);
    cleanupWinsock();
    return 0;
}
