#include <string>
#include <iostream>
#include <stdint.h>
#include <QtNetwork/QTcpSocket>

#include "logger.h"
#include "netwrapper.h"

using namespace std;

namespace remcu {

static QTcpSocket* socket = new QTcpSocket();

static int timeout_ms = 0;

bool connectTCP(const std::string host, const uint16_t port, const int timeout_sec){
    if(socket == nullptr){
        ADIN_LOG(__ERROR) << "socket is not existing";
        return false;
    }

    timeout_ms = timeout_sec*1000;

    socket->abort();
    socket->connectToHost(host.c_str(), port);

    if (socket->waitForConnected(timeout_ms) == false){
        ADIN_LOG(__ERROR) << "timeout connection";
        return false;
    }

    ADIN_LOG(__INFO) << "Connection success " << host << ", port : " << port ;
    return true;
}

bool closeTCP(){
    if(socket->state() == QAbstractSocket::UnconnectedState ||
        socket->state() == QAbstractSocket::ClosingState){
        ADIN_LOG(__ERROR) << "socket is disconnected already";
        return true;
    }

    socket->abort();

    return true;
}

bool send(const char * buffer, const size_t lenBuffer){

    ADIN_LOG(__DEBUG) << "-> " << lenBuffer << " : '" << buffer <<"'";

    if(socket->state() != QAbstractSocket::ConnectedState){
        ADIN_LOG(__ERROR) << "socket is disconnected already";
        return false;
    }

    const qint64 len = socket->write(buffer, static_cast<qint64>(lenBuffer));

    if(socket->waitForBytesWritten(timeout_ms) == false){
        ADIN_LOG(__ERROR) << "writing socket timeout";
        return false;
    }

    if(len == static_cast<qint64>(lenBuffer)){
        return true;
    }

    ADIN_LOG(__ERROR) << "sended bytes : " << len << " instead : " << lenBuffer;

    return false;
}

bool receive(char * buffer, size_t & lenBuffer){

    if(socket->state() != QAbstractSocket::ConnectedState){
        ADIN_LOG(__ERROR) << "socket is disconnected already";
        return false;
    }

    if(socket->waitForReadyRead(timeout_ms) == false){
        ADIN_LOG(__ERROR) << "reading socket timeout";
        return false;
    }

    const QByteArray socketBuf = socket->readAll();

    if(socketBuf.size() > static_cast<int>(lenBuffer - 1)){
        ADIN_LOG(__ERROR) << "readable buffer is small";
        return false;
    }

    if(socketBuf.size() <= 0){
        ADIN_LOG(__ERROR) << "reading socket failed";
        return false;
    }

    lenBuffer = static_cast<size_t>(socketBuf.size());
    memcpy(buffer, socketBuf.data(), lenBuffer);
    buffer[lenBuffer] = '\0';

    ADIN_LOG(__DEBUG) << "<- " << lenBuffer << " : '" << buffer <<"'";
    return true;
}

} //namespace
