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

void handleClient(SOCKET client_socket) {
    
}

int main()
{
}


