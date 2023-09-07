#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>

#include "assertion.h"
#include "netwrapper.h"
#include "logger.h"
#include "BaseClient.h"



using namespace std;


namespace remcu {

vector<char> ClientBase::bufferSend(0x100,'\0');
vector<char> ClientBase::bufferReceiv(0x100,'\0');

bool ClientBase::connect(const std::string host, const uint16_t port, const int timeout_sec) const {
    if(connectTCP(host, port, timeout_sec)){
        if(this->provisioning())
            return true;
        else
            this->close();
    }
    return false;
}

bool ClientBase::provisioning() const {
    return true;
}

bool ClientBase::close() const {
    return closeTCP();
}

bool readBeforeToken(vector<char> & buffer, size_t & lenResp, const char token){
    size_t len = 0;
    size_t respN = 0;
    char * p = buffer.data();

    while(true){
        len = buffer.size();
        assert_1message(receiveResponseFromServer(p + respN, len),
                        "server failed - timeout");

        respN += len;
        buffer.at(respN) = '\0';

        assert_printf(respN < buffer.size(),
                      "receive buffer is small: %d > %d\n", respN , buffer.size());

        ADIN_PRINTF(__DEBUG, "buf %d : %s\n", respN, buffer.data());

        if(strchr(p, token) != nullptr){
            ADIN_PRINTF(__DEBUG, "token got %c : %s\n", token, buffer.data());
            break;
        }
    }

    lenResp = respN;

    return true;
}


bool commandSendAndGetResponse(const char * data, const size_t lenData,
                                      vector<char> & bufferResp, size_t & lenResp, const char token){
    assert_1message(lenData > 0, "transmitted buffer is empty");

    assert_1message(sendMessage2Server(data, lenData), "can't transmite to server");

    assert_1message(readBeforeToken(bufferResp, lenResp, token), "server don't respond");

    bufferResp.at(lenResp) = '\0';

    return true;
}

} //namespace
