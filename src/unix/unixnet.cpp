#include <string>
#include <iostream>
#include <stdint.h>

#include "logger.h"
#include "unix/tcpconnector.h"
#include "netwrapper.h"


using namespace std;
using namespace unix_tcp;

namespace remcu {

static TCPConnector* connector = new TCPConnector();

static TCPStream* stream = NULL;

static int current_timeout_sec = -1;


bool connectTCP(const std::string host, const uint16_t port, const int timeout_sec){

    if(connector == NULL){
        ADIN_LOG(__ERROR) << _S_("internal error");
        return false;
    }

    closeTCP();

    current_timeout_sec = timeout_sec;

    stream = connector->connect(host.c_str(), port, current_timeout_sec);

    if(stream == NULL){
        ADIN_LOG(__ERROR) << "Failed " << host << ", port : " << port ;
        return false;
    }

    ADIN_LOG(__INFO) << _S_("Connection success ") << host << ", port : " << port ;
    return true;
}

bool closeTCP(){
    if(stream != NULL){
        ADIN_LOG(__INFO) << _S_("close connection");
        delete stream;
        stream = NULL;
    }

    return true;
}


bool sendTCP(const char * buffer, const size_t lenBuffer){

    if(stream == NULL){
        ADIN_LOG(__ERROR) << _S_("Connection close yet ");
        return false;
    }

    const ssize_t len = stream->send(buffer, lenBuffer);
    if( len == static_cast<ssize_t>(lenBuffer)){
        return true;
    }

    ADIN_PRINTF(__ERROR, "size : %d | send %d\n", lenBuffer, len);

    return false;
}

bool receive(char * buffer, size_t & lenBuffer){

    if(stream == NULL){
        ADIN_LOG(__ERROR) << _S_("Connection close yet ");
        return false;
    }

    const ssize_t lenSocket = stream->receive(buffer, lenBuffer, current_timeout_sec);

    if(lenSocket <= 0){
        ADIN_LOG(__ERROR) << _S_("receive response failed, len:") << lenSocket ;
        return false;
    }

    if(lenSocket > static_cast<ssize_t>(lenBuffer - 1)){
        ADIN_LOG(__ERROR) << _S_("readable buffer is small");
        return false;
    }

    buffer[lenSocket] = '\0';

    lenBuffer = static_cast<size_t>(lenSocket);

    return true;
}

} //namespace
