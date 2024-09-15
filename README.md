# Chat Application

A simple console-based chat application written in C++ that allows multiple clients to connect to a server and send messages to each other in real-time using sockets (Winsock2). This project includes both server and client programs, as well as utility functions for handling socket communication.

## Features

- Multiple clients can connect to a single server.
- Messages are broadcast from one client to all connected clients.
- Threaded client handling for concurrent connections.
- Basic error handling for socket communication.

## Prerequisites

To compile and run this project, you need:

- **Visual Studio** with C++ development tools.
- Windows operating system (for Winsock2 support).
- Basic knowledge of socket programming.

# Installation and Setup

## 1. Open in Visual Studio
  1. Launch in Visual Studio
  2. Add the following files
     - ChatAppServer.cpp
     - ChatAppClient.cpp
     - ChatAppUtils.h
       
## 2. Build and Configure Project
  1. Add Winsock2 library
     - Right-click on the project in the Solution Explorer and select Properties
     - Under Configuration Properties → Linker → Input, add Ws2_32.lib to the Additional Dependencies field.
  2. Set Startup Project:
     - If you're running the server, right-click ChatAppServer.cpp and select Set as Startup Project.
     - If you're running the client, right-click ChatAppClient.cpp and select Set as Startup Project.

## 3. Build the Project
  1. Go to the Build menu and click Build Solution or press Ctrl+Shift+B.
  2. Check for any errors or warnings in the Output or Error List.

# Running the Application

## Running the Server
  1 .Set the ChatAppServer.cpp as the startup project.
  2. Press F5 to run the server. The server will start listening for connections on 127.0.0.1:8080.

## Running the Client
  1. Set the ChatAppClient.cpp as the startup project.
  2.Press F5 to run the client. The client will connect to the server at 127.0.0.1:8080.
  3. You can open multiple client instances to simulate multiple users.
     
## Sending Messages
  1. Once connected, you can send a message by typing into the console and pressing Enter.
  2. Messages will be broadcast to all connected clients, and each client will see messages from other clients.
     
## Closing the Application
  !. To stop the server or client, press Ctrl+C or close the console window.

# Future Improvements
 - Add user authentication.
 - Encrypt messages using SSL/TLS.
 - Improve error handling and stability.
 - Implement a graphical user interface (GUI) for easier interaction.
  
