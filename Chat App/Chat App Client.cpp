//Chat App Client.cpp
#include <iostream>
#include <thread>
#include <cstring>
#include <winsock.h>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib") // links winsock library

//function to receive and print messages from the server
void receiveMessages() {
	char buffer[1024];
	while (true) {
		memset(buffer, 0, sizeof(buffer)); //clear the buffer
		int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0); //receive data from server
		if (bytes_received > 0) {
			std::cout << "Message received: " << buffer << std::endl; //print received message
		}
	}
}

int main() {
    WSADATA wsaData;
    //initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return -1;
    }

    SOCKET client_socket;
    struct sockaddr_in server_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);  //create a socket
    server_addr.sin_family = AF_INET;  //set address family to IPv4
    server_addr.sin_port = htons(8080);  //set the port number
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);  //convert and set the IP address (localhost)

    //connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed\n";
        return -1;
    }

    //start a thread to receive messages from the server
    std::thread(receiveMessages, client_socket).detach();

    std::string message;
    while (true) {
        std::getline(std::cin, message);  //read user input
        send(client_socket, message.c_str(), message.size(), 0);  //send the message to the server
    }

    closesocket(client_socket);  //close the client socket
    WSACleanup();  //clean up Winsock
    return 0;
}