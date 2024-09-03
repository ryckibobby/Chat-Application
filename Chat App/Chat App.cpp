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
        broadcastMessage(message, client_socket); // broadcast the received message to other clients
    }
}

int main() {

}


