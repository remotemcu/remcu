#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>

#include "assertion.h"
#include "netwrapper.h"
#include "logger.h"
#include "client.h"



using namespace std;


namespace adin {

vector<char> ClientBase::bufferSend(0x100,'\0');
vector<char> ClientBase::bufferReceiv(0x100,'\0');

bool ClientBase::connect(const std::string host, const uint16_t port, const int timeout_sec) const {
    return connectTCP(host, port, timeout_sec);
}

bool ClientBase::close() const {
    return closeTCP();
}

static bool readBeforeToken(vector<char> & buffer, size_t & lenResp, const char token){
    size_t len = 0;
    size_t respN = 0;
    char * p = buffer.data();

    while(true){
        len = buffer.size();
        asser_1line(receiveResponseFromServer(p + respN, len));

        respN += len;
        buffer.at(respN) = '\0';

        asser_1line(respN < buffer.size());

        ADIN_PRINTF(_DEBUG, "> %d - %s\n", respN, buffer.data());

        if(strchr(p, token) != nullptr){
            break;
        }
    }

    lenResp = respN;

    return true;
}


bool commandSendAndGetResponse(const char * data, const size_t lenData,
                                      vector<char> & bufferResp, size_t & lenResp, const char token){
    asser_1line(lenData > 0 || !"transmitted buffer is empty");

    asser_1line(sendMessage2Server(data, lenData));

    asser_1line(readBeforeToken(bufferResp, lenResp, token));

    bufferResp.at(lenResp) = '\0';

    return true;
}

} //namespace
