#include <cstdio>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <assert.h>
#include <cstring>


#include "netwrapper.h"
#include "ocdcommand.h"


using namespace std;


namespace ocd_lib {

#define COMMAND_TOKEN '\x1a'

static const char * loadTempCommand = "ocd_m%c%c 0x%lX\x1a";
static const char * storeTempCommand = "m%c%c 0x%lX 0x%lX\x1a";

enum TypeOperation_t {
    STORE = 'w', LOAD = 'd'
};


enum SizeOperation_t {
    BYTE = 'b', HALF_WORD = 'h', WORD = 'w'
};

static vector<char> bufferSend(0x100,'\0');
static vector<char> bufferReceiv(0x100,'\0');


static bool getMaskAndSize(llvm_pass_arg sizeVal, llvm_pass_arg & mask, char & sizeOp){
    switch (sizeVal) {
    case 1:
        mask = 0x1;
        sizeOp = BYTE;
        break;
    case 8:
        mask = 0xFF;
        sizeOp = BYTE;
        break;
    case 16:
        mask = 0xFFFF;
        sizeOp = HALF_WORD;
        break;
    case 32:
        mask = 0xFFFFFFFF;
        sizeOp = WORD;
        break;
    default:
        assert(!"Unrichable size!");
        break;
    }

    return true;
}

 static bool readBeforeToken(vector<char> & buffer, size_t & len){
    bool success = false;
    size_t lenResp;
    size_t respN = 0;
    char * p = buffer.data();

    while(true){
        lenResp = buffer.size();
        assert(receiveOCDResponse(p + respN, lenResp));

        respN += lenResp;
        buffer.at(respN) = '\0';

        assert(respN < buffer.size());

        printf("> %d - %s\n", respN, buffer.data());

        if(strchr(p, COMMAND_TOKEN) != NULL){
            break;
        }
    }

    len = respN;

    return true;
}

static bool parseValue(vector<char> & buffer, llvm_pass_arg & value){

    char * p = buffer.data();

    if(strtok(p,":") == NULL)
        return false;
    const char * valp = strtok(NULL,": ");

    if(valp == NULL){
        return false;
    }

    value = strtoul(valp, NULL, 16);

    return true;
}


static bool commandSendAndResponse(char * data, size_t lenData,
                                   vector<char> & bufferResp, size_t & lenResp){
    assert(lenData > 0 || !"buffer size less");

    assert(sendTCLMessage2OCD(data, lenData));

    assert(readBeforeToken(bufferReceiv, lenResp));

    bufferReceiv.at(lenResp) = '\0';

    return true;
}


bool store2RemoteAddr(llvm_ocd_addr addr, llvm_pass_arg value, llvm_pass_arg sizeVal){

    llvm_pass_arg mask = 0;
    char sizeOp = 0;

    assert(getMaskAndSize(sizeVal, mask, sizeOp));

    const llvm_pass_arg sendValue = value & mask;

    const int len = snprintf(bufferSend.data(), bufferSend.size(),
                    storeTempCommand,
                             STORE, sizeOp, addr, sendValue);

    size_t lenBuf;
    commandSendAndResponse(bufferSend.data(), len, bufferReceiv, lenBuf);

    return true;
}

bool loadFromRemoteAddr(llvm_ocd_addr addr, llvm_pass_arg & value, llvm_pass_arg sizeVal){

    llvm_pass_arg mask = 0;
    char sizeOp = 0;

    assert(getMaskAndSize(sizeVal, mask, sizeOp));


    const int len = snprintf(bufferSend.data(), bufferSend.size(),
                             loadTempCommand,
                             LOAD, sizeOp, addr);

    size_t lenBuf = 0;
    commandSendAndResponse(bufferSend.data(), len, bufferReceiv, lenBuf);

    assert(parseValue(bufferReceiv, value));

    return true;
}


} //namespace
