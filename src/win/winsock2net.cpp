#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>

#include "logger.h"
#include "netwrapper.h"


#pragma clang diagnostic ignored "-Wdeprecated-declarations"


namespace remcu {

static SOCKET ConnectSocket = INVALID_SOCKET;

bool connectTCP(const std::string host, const uint16_t port, const int timeout_sec){

    int ret;
    WSADATA wsaData;
    SOCKADDR_IN          ServerAddr;// = {0};

    memset(&ServerAddr, 0, sizeof(SOCKADDR_IN));

    const int timeout_ms = timeout_sec*1000;

    // Initialize Winsock
    ret = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (ret != 0) {
        ADIN_PRINTF(__ERROR, "Startup failed: %d\n", ret);
        return false;
    }

    ADIN_PRINTF(__INFO, "Client: BBsock status is %s.\n", wsaData.szSystemStatus);

    ConnectSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(ConnectSocket == INVALID_SOCKET){
          ADIN_PRINTF(__ERROR, "Client: socket() failed! Error code: %ld\n", WSAGetLastError());
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
     ret = connect(ConnectSocket, reinterpret_cast<SOCKADDR *>(&ServerAddr), sizeof(ServerAddr));
     if(ret != 0){
         ADIN_PRINTF(__ERROR, "Client: connect() failed! Error code: %ld\n", WSAGetLastError());
           // Close the socket
           closesocket(ConnectSocket);
           // Do the clean up
           WSACleanup();
           // Exit with error
           return false;
     }
     ADIN_LOG(__INFO) << "Client: connect() is OK, got connected...";
     ADIN_LOG(__INFO) << "Client: Ready for sending and/or receiving data...";
     // At this point you can start sending or receiving data on
    // the socket SendingSocket.
    // Some info on the receiver side...
     getsockname(ConnectSocket, reinterpret_cast<SOCKADDR *>(&ServerAddr),
                 reinterpret_cast<int *>(sizeof(ServerAddr)));
     ADIN_PRINTF(__INFO, "Client: Receiver IP(s) used: %s\n", inet_ntoa(ServerAddr.sin_addr));
     ADIN_PRINTF(__INFO, "Client: Receiver port used: %d\n", htons(ServerAddr.sin_port));

     ret = setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&timeout_ms)
                      , sizeof(int));
        if (ret == SOCKET_ERROR) {
           ADIN_PRINTF(__WARNING, "setsockopt for receive timeout failed with error: %u\n", WSAGetLastError());
        } else
            ADIN_PRINTF(__INFO, "Set receive timeout: ON\n",0);

    ret = setsockopt(ConnectSocket, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char *>(&timeout_ms)
                     , sizeof(int));
       if (ret == SOCKET_ERROR) {
          ADIN_PRINTF(__WARNING, "setsockopt for transmite timeout failed with error: %u\n", WSAGetLastError());
       } else
           ADIN_PRINTF(__INFO, "Set transmite timeout : ON\n",0);

     return true;
}


bool closeTCP(){
    bool success = true;
    if( shutdown(ConnectSocket, SD_SEND) != 0){
              ADIN_PRINTF(__WARNING, "Client: Well, there is something wrong with the shutdown().\
                 The error code: %ld\n", WSAGetLastError());
                     success = false;
    } else {
        ADIN_LOG(__DEBUG) << "Client: shutdown() looks OK...";
    }

     // When you are finished sending and receiving data on socket SendingSocket,
     // you should close the socket using the closesocket API. We will
     // describe socket closure later in the chapter.
     if(closesocket(ConnectSocket) != 0){
          ADIN_PRINTF(__WARNING, "Client: Cannot close \"Socket\" socket. Error code: %ld\n", WSAGetLastError());
          success = false;
     } else {
        ADIN_LOG(__DEBUG) << "Client: Closing \"Socket\" socket...";
     }
     // When your application is finished handling the connection, call WSACleanup.
     if(WSACleanup() != 0){
          ADIN_PRINTF(__WARNING, "Client: BBCleanup() failed!...\n",0);
          success = false;
     } else {
        ADIN_LOG(__DEBUG) << "Client: BBCleanup() is OK...";
     }
     return false;
}

bool sendTCP(const char * buffer, const size_t lenBuffer){

    if(ConnectSocket == INVALID_SOCKET){
          ADIN_PRINTF(__ERROR, "Client: socket() failed! Error code: %ld\n", WSAGetLastError());
          return false;
     }

    // Send an initial buffer
    const int ret = send(ConnectSocket, buffer, static_cast<int>(lenBuffer), 0);
    if (ret == SOCKET_ERROR) {
        ADIN_PRINTF(__ERROR, "send failed: %d\n", WSAGetLastError());
        return false;
    }

    return true;
}

bool receive(char * buffer, size_t & lenBuffer){

    if(ConnectSocket == INVALID_SOCKET){
          ADIN_PRINTF(__ERROR, "Client: socket() failed! Error code: %ld\n", WSAGetLastError());
          return false;
     }

    const int ret = recv(ConnectSocket, buffer, static_cast<int>(lenBuffer), 0);
        if (ret > 0){
            ADIN_PRINTF(__DEBUG, "Bytes received: %d\n", ret);
            lenBuffer = static_cast<size_t>(ret);
            return true;
        } else if (ret == 0)
            ADIN_PRINTF(__ERROR, "Connection closed\n",0);
        else
            ADIN_PRINTF(__ERROR, "recv failed: %d\n", WSAGetLastError());

        return false;
}

} //namespace
