#include <string>
#include <iostream>
#include <stdint.h>
#include <QtNetwork/QTcpSocket>

#include "unix/tcpconnector.h"
#include "logger.h"
#include "netwrapper.h"

using namespace std;

namespace adin {

static QTcpSocket* socket = new QTcpSocket();

static int timeout_ms = 0;

static bool isHostNotConnected(){
    if(socket->state() == QAbstractSocket::UnconnectedState ||
        socket->state() == QAbstractSocket::ClosingState){
        return true;
    }
    return false;
}

bool connectTCP(const std::string host, const uint16_t port, const int timeout){
    if(socket == NULL){
        ADIN_LOG(_ERROR) << "socket is not existing";
        return false;
    }

    timeout_ms = timeout;

    socket->abort();
    socket->connectToHost(host.c_str(), port);

    if (socket->waitForConnected(timeout) == false){
        ADIN_LOG(_ERROR) << "timeout connection";
        return false;
    }

    ADIN_LOG(_INFO) << "Connection success " << host << ", port : " << port ;
    return true;
}

bool closeTCP(){
    if(isHostNotConnected()){
        ADIN_LOG(_ERROR) << "socket is disconnected already";
        return true;
    }

    socket->abort();

    return true;
}


} //namespace
