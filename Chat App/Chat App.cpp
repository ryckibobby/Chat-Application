// Chat App.cpp 
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <algorithm>
#include <cstring>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib") //links winsock library

//global variables to store connected clients and mutex for thread-safe access
std::vector<SOCKET> clients;
std::mutex clients_mutex;

//broadcast messages to all clients
void broadcastMessage(const std::string& message, SOCKET sender) {
    std::lock_guard<std::mutex> lock(clients_mutex);  
    for (SOCKET client : clients) {
        if (client != sender) {
            send(client, message.c_str(), message.size(), 0);  // send message to the client
        }
    }
}

//function to handle communication with each client
void handleClient(SOCKET client_socket) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            //client disconnected or an error occurred
            std::lock_guard<std::mutex> lock(clients_mutex); //lock clients vector
            clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end()); //remove client
            closesocket(client_socket);
            break;
        }
        std::string message(buffer); //convert buffer to string
        broadcastMessage(message, client_socket); //broadcast the received message to other clients
    }
}

int main() {
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return -1;
    }

    SOCKET server_socket;
    struct sockaddr_in server_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;  
    server_addr.sin_port = htons(8080);  
    server_addr.sin_addr.s_addr = INADDR_ANY;  //accept connections from any IP address

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));  //bind the socket to the address
    listen(server_socket, 5);  //listen for incoming connections, with a backlog of 5

    std::cout << "Server started on port 8080\n";

    while (true) {
        SOCKET client_socket = accept(server_socket, nullptr, nullptr);  //accept a new client connection
        std::lock_guard<std::mutex> lock(clients_mutex);  //lock the clients vector
        clients.push_back(client_socket);  //add the new client to the list
        std::thread(handleClient, client_socket).detach();  //start a new thread to handle the client
    }

    closesocket(server_socket);  //close the server socket
    WSACleanup();  //clean up Winsock
    return 0;
}


