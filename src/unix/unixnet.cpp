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


bool connectTCP(std::string host, uint16_t port, int timeout){

    if(connector == NULL){
        ADIN_LOG(_ERROR) << "internal error";
        return false;
    }

    closeTCP();

    stream = connector->connect(host.c_str(), port, timeout);

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


bool sendMessage2Server(char * buffer, size_t lenBuffer){

    ADIN_LOG(_DEBUG) << "-> " << " len: " << lenBuffer << " : " << buffer;

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

bool receiveResponseFromServer(char * buffer, size_t & lenBuffer, int timeout_sec){

    if(stream == NULL){
        ADIN_LOG(_ERROR) << "Connection close yet ";
        return false;
    }

    const ssize_t len = stream->receive(buffer, lenBuffer, timeout_sec);

    if(len > 0){
        lenBuffer = len;
        return true;
    }

    ADIN_LOG(_ERROR) << "receive response failed, len:" << len ;
    return false;
}

} //namespace
