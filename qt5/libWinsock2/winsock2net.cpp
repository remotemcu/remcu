#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>

#include "logger.h"
#include "netwrapper.h"

namespace adin {

static SOCKET ConnectSocket = INVALID_SOCKET;

bool connectTCP(const std::string host, const uint16_t port, const int timeout_sec){

    int ret;
    WSADATA wsaData;
    struct addrinfo *result = NULL,
                        *ptr = NULL,
                        hints;
    SOCKADDR_IN          ServerAddr, ThisSenderInfo;

    // Initialize Winsock
    ret = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (ret != 0) {
        printf("WSAStartup failed: %d\n", ret);
        return false;
    }

    printf("Client: Winsock DLL status is %s.\n", wsaData.szSystemStatus);

    ConnectSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(ConnectSocket == INVALID_SOCKET){
          printf("Client: socket() failed! Error code: %ld\n", WSAGetLastError());
          // Do the clean up
          WSACleanup();
          // Exit with error
          return false;
     }
    // Set up a SOCKADDR_IN structure that will be used to connect
     // to a listening server on port 5150. For demonstration
     // purposes, let's assume our server's IP address is 127.0.0.1 or localhost
     // IPv4
     ServerAddr.sin_family = AF_INET;
     // Port no.
     ServerAddr.sin_port = htons(port);
     // The IP address
     ServerAddr.sin_addr.s_addr = inet_addr(host.c_str());
    // Make a connection to the server with socket SendingSocket.
     ret = connect(ConnectSocket, (SOCKADDR *) &ServerAddr, sizeof(ServerAddr));
     if(ret != 0){
         printf("Client: connect() failed! Error code: %ld\n", WSAGetLastError());
           // Close the socket
           closesocket(ConnectSocket);
           // Do the clean up
           WSACleanup();
           // Exit with error
           return -1;
     }
     printf("Client: connect() is OK, got connected...\n");
     printf("Client: Ready for sending and/or receiving data...\n");
     // At this point you can start sending or receiving data on
    // the socket SendingSocket.
    // Some info on the receiver side...
     getsockname(ConnectSocket, (SOCKADDR *)&ServerAddr, (int *)sizeof(ServerAddr));
     printf("Client: Receiver IP(s) used: %s\n", inet_ntoa(ServerAddr.sin_addr));
     printf("Client: Receiver port used: %d\n", htons(ServerAddr.sin_port));

     return true;
}


bool closeTCP(){
    bool success = true;
    if( shutdown(ConnectSocket, SD_SEND) != 0){
              printf("Client: Well, there is something wrong with the shutdown().\
                 The error code: %ld\n", WSAGetLastError());
                     success = false;
    } else {
        printf("Client: shutdown() looks OK...\n");
    }

     // When you are finished sending and receiving data on socket SendingSocket,
     // you should close the socket using the closesocket API. We will
     // describe socket closure later in the chapter.
     if(closesocket(ConnectSocket) != 0){
          printf("Client: Cannot close \"SendingSocket\" socket. Error code: %ld\n", WSAGetLastError());
          success = false;
     } else {
        printf("Client: Closing \"SendingSocket\" socket...\n");
     }
     // When your application is finished handling the connection, call WSACleanup.
     if(WSACleanup() != 0){
          printf("Client: WSACleanup() failed!...\n");
          success = false;
     } else {
        printf("Client: WSACleanup() is OK...\n");
     }
     return false;
}

bool sendMessage2Server(const char * buffer, const size_t lenBuffer){

    if(ConnectSocket == INVALID_SOCKET){
          printf("Client: socket() failed! Error code: %ld\n", WSAGetLastError());
          return false;
     }

    // Send an initial buffer
    const int ret = send(ConnectSocket, buffer, lenBuffer, 0);
    if (ret == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        return false;
    }

    return true;
}

bool receiveResponseFromServer(char * buffer, size_t & lenBuffer){

    if(ConnectSocket == INVALID_SOCKET){
          printf("Client: socket() failed! Error code: %ld\n", WSAGetLastError());
          return false;
     }

    const int ret = recv(ConnectSocket, buffer, lenBuffer, 0);
        if (ret > 0){
            printf("Bytes received: %d\n", ret);
            lenBuffer = ret;
            return true;
        } else if (ret == 0)
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());

        return false;
}

} //namespace
