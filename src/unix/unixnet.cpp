#include <string>
#include <iostream>
#include <stdint.h>

#include "logger.h"
#include "unix/tcpconnector.h"
#include "netwrapper.h"


using namespace std;
using namespace unix_tcp;

namespace adin {

static TCPConnector* connector = new TCPConnector();

static TCPStream* stream = NULL;

static int current_timeout_sec = -1;


bool connectTCP(const std::string host, const uint16_t port, const int timeout_sec){

    if(connector == NULL){
        ADIN_LOG(_ERROR) << "internal error";
        return false;
    }

    closeTCP();

    current_timeout_sec = timeout_sec;

    stream = connector->connect(host.c_str(), port, current_timeout_sec);

    if(stream == NULL){
        ADIN_LOG(_ERROR) << "Failed " << host << ", port : " << port ;
        return false;
    }

     ADIN_LOG(_INFO) << "Connection success " << host << ", port : " << port ;
    return true;
}

bool closeTCP(){
    if(stream != NULL){
        ADIN_LOG(_INFO) << "close connection";
        delete stream;
    }

    return true;
}


bool sendMessage2Server(const char * buffer, const size_t lenBuffer){

    ADIN_LOG(_DEBUG) << "-> " << lenBuffer << " : '" << buffer <<"'";

    if(stream == NULL){
        ADIN_LOG(_ERROR) << "Connection close yet ";
        return false;
    }

    const ssize_t len = stream->send(buffer, lenBuffer);
    if( len == static_cast<ssize_t>(lenBuffer)){
        return true;
    }

    ADIN_LOG(_ERROR) << "send message failed " << len;

    return false;
}

bool receiveResponseFromServer(char * buffer, size_t & lenBuffer){

    if(stream == NULL){
        ADIN_LOG(_ERROR) << "Connection close yet ";
        return false;
    }

    const ssize_t lenSocket = stream->receive(buffer, lenBuffer, current_timeout_sec);

    if(lenSocket <= 0){
        ADIN_LOG(_ERROR) << "receive response failed, len:" << lenSocket ;
        return false;
    }

    if(lenSocket > static_cast<ssize_t>(lenBuffer - 1)){
        ADIN_LOG(_ERROR) << "readable buffer is small";
        return false;
    }

    buffer[lenSocket] = '\0';

    lenBuffer = lenSocket;
    ADIN_LOG(_DEBUG) << "<- " << lenBuffer << " : '" << buffer <<"'";

    return true;
}

} //namespace