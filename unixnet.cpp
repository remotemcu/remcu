#include "netwrapper.h"
#include <string>
#include <iostream>
#include <stdint.h>

#include "tcpconnector.h"

using namespace std;
using namespace unix_tcp;

namespace ocd_lib {

static TCPConnector* connector = new TCPConnector();

static TCPStream* stream = NULL;

static LevelDebug _LEVEL;


void setConnectionVerboseLevel(LevelDebug level);


bool connect2OpenOcd(std::string host, uint16_t port, int timeout_sec, bool verbose){

    if(connector == NULL){
        cout << "internal error" << endl;
        return false;
    }

    if(stream != NULL){
        cout << "close connection" << endl;
        delete stream;
    }

    stream = connector->connect(host.c_str(), port, timeout_sec);

    if(stream == NULL){
        cout << "Connection failed " << host << endl;
        return false;
    }

    cout << "Connection success " << host << endl;
    return true;
}


//bool sendTCLMessage2OCD(std::vector<char> & message){
bool sendTCLMessage2OCD(char * buffer, size_t lenBuffer){

    cout << "-> " << " len: " << lenBuffer << " : " << buffer  << endl;

    if(stream == NULL){
        cout << "Connection close yet "  << endl;
        return false;
    }

    const ssize_t len = stream->send(buffer, lenBuffer);
    if( len == static_cast<ssize_t>(lenBuffer)){
        return true;
    }

    cout << "sendTCLMessage2OCD failed " << len << endl;

    return false;
}

bool receiveOCDResponse(char * buffer, size_t & lenBuffer, int timeout_sec){

    if(stream == NULL){
        cout << "Connection close yet "  << endl;
        return false;
    }

    const ssize_t len = stream->receive(buffer, lenBuffer, timeout_sec);

    if(len > 0){
        lenBuffer = len;
        return true;
    }

    cout << "receiveOCDResponse failed " << len  << endl;
    return false;
}

} //namespace
