#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>

#include "assertion.h"
#include "netwrapper.h"
#include "OpenocdClient.h"
#include "logger.h"
#include "client.h"

using namespace std;


namespace adin {

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


static bool getMaskAndSize(const llvm_pass_arg sizeVal, llvm_pass_arg & mask, char & sizeOp){
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
        asser_1line(!"Unrichable size!");
        break;
    }

    return true;
}

 static bool readBeforeToken(vector<char> & buffer, size_t & lenResp){
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

        if(strchr(p, COMMAND_TOKEN) != NULL){
            break;
        }
    }

    lenResp = respN;

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


static bool commandSendAndResponse(const char * data, const size_t lenData,
                                   vector<char> & bufferResp, size_t & lenResp){
    asser_1line(lenData > 0 || !"buffer size less");

    asser_1line(sendMessage2Server(data, lenData));

    asser_1line(readBeforeToken(bufferResp, lenResp));

    bufferReceiv.at(lenResp) = '\0';

    return true;
}

bool ClientOpenOCD::connect(const std::string host, const uint16_t port, const int timeout) const {
    return connectTCP(host, port, timeout);
}

bool ClientOpenOCD::close() const {
    return closeTCP();
}

bool ClientOpenOCD::resetRemoteUnit(const ResetType type) const {
    string reset_message("reset ");

    switch (type) {
    case ResetType::__RUN:
        reset_message.append("run");
        break;
    case ResetType::__HALT:
        reset_message.append("halt");
        break;
    case ResetType::__INIT:
        reset_message.append("init");
        break;
    default:
        return false;
    }

    reset_message.push_back('\x1a');

    size_t lenReceiv;
    return commandSendAndResponse(reset_message.c_str(), reset_message.size(), bufferReceiv, lenReceiv);
}

bool ClientOpenOCD::store2RemoteAddr(const llvm_ocd_addr addr, const llvm_pass_arg value, const llvm_pass_arg sizeVal) const {

    llvm_pass_arg mask = 0;
    char sizeOp = 0;

    asser_1line(getMaskAndSize(sizeVal, mask, sizeOp));

    const llvm_pass_arg sendValue = value & mask;

    const int len = snprintf(bufferSend.data(), bufferSend.size(),
                    storeTempCommand,
                             STORE, sizeOp, addr, sendValue);

    size_t lenReceiv;
    commandSendAndResponse(bufferSend.data(), len, bufferReceiv, lenReceiv);

    return true;
}

bool ClientOpenOCD::loadFromRemoteAddr(const llvm_ocd_addr addr, llvm_pass_arg & value, const llvm_pass_arg sizeVal) const {

    llvm_pass_arg mask = 0;
    char sizeOp = 0;

    asser_1line(getMaskAndSize(sizeVal, mask, sizeOp));


    const int len = snprintf(bufferSend.data(), bufferSend.size(),
                             loadTempCommand,
                             LOAD, sizeOp, addr);

    size_t lenReceiv = 0;
    commandSendAndResponse(bufferSend.data(), len, bufferReceiv, lenReceiv);

    asser_1line(parseValue(bufferReceiv, value));

    return true;
}

static const char * array2memTemplate = "array unset MASS; array set MASS { %s }; array2mem MASS %d 0x%x %d\x1a";
static const char * arrayFirstPart = "array unset MASS; array set MASS {";
static const char * arraySecondPart = " }; array2mem MASS 8 0x%x %d\x1a";
static const char * elementTemplate = " %d 0x%x";
const size_t sizeBuf = 200;
static char buf[sizeBuf];

static vector<char> arrayBuffer(100);

bool ClientOpenOCD::fastWrite2RemoteMem(const uintptr_t addr, const char* sink, const size_t size) const {

    const size_t potential_array_size = size*6;
    const size_t _ADD = 100;

    int len = -1;
    size_t usedBufferBytes = 0;

    if(arrayBuffer.size() < potential_array_size){
        arrayBuffer.resize(potential_array_size);
    }

    for(size_t i =0; i < size; i++){
        len = snprintf(buf, sizeBuf, elementTemplate, i, sink[i]);
        asser_1line(len > 0);
        const size_t future_size = usedBufferBytes + len;
        if(arrayBuffer.size() <= future_size){
            arrayBuffer.resize(future_size + _ADD);
        }
        strncpy(arrayBuffer.data() + usedBufferBytes, buf, len);
        usedBufferBytes += len;
    }

    const size_t potential_size_buffer = arrayBuffer.size() + strlen(array2memTemplate) + _ADD;
    if(bufferSend.size() < potential_size_buffer){
        bufferSend.resize(potential_size_buffer + _ADD);
    }

    strncpy(bufferSend.data(), arrayFirstPart, strlen(arrayFirstPart));

    size_t pos = strlen(arrayFirstPart);

    strncpy(bufferSend.data() + pos, arrayBuffer.data(), usedBufferBytes);

    pos += usedBufferBytes;

    len = snprintf(buf, sizeBuf, arraySecondPart, addr, size);
    asser_1line(len > 0);
    strncpy(bufferSend.data() + pos, buf, len);

    pos += len;

    size_t lenBufReceiv;
    commandSendAndResponse(bufferSend.data(), pos, bufferReceiv, lenBufReceiv);

    return true;
}

static const char * loadArrayTempCommand = "ocd_mdb 0x%lX %d\x1a";
static const size_t qty_value_in_row = 32;
static const size_t qty_service_bytes = 12;
static const size_t _GAP = 100;
static const char _SEPARATOR_LOAD = ' ';

bool ClientOpenOCD::fastLoadFromRemoteMem(const uintptr_t addr, const size_t size, char* dist) const {

    const size_t potential_size_receive = size*3 +
        (size/qty_value_in_row)*qty_service_bytes + _GAP;

    if(bufferReceiv.size() < potential_size_receive){
        bufferReceiv.resize(potential_size_receive + _GAP);
    }

    const int len = snprintf(bufferSend.data(), bufferSend.size(),
                             loadArrayTempCommand,
                             addr, size);

    size_t lenBuf = 0;
    commandSendAndResponse(bufferSend.data(), len, bufferReceiv, lenBuf);

    char * point = bufferReceiv.data();

    char buf[10];
    size_t pos = 0;

    for(size_t i = 0; i < size; i++){
        for( ; point[pos] != _SEPARATOR_LOAD; pos++);
        pos++;
        buf[0] = point[pos];
        pos++;
        buf[1] = point[pos];
        pos++;
        buf[2] = '\0';
        dist[i] = strtoul(buf, NULL, 16) & 0xFF;
    }

    return true;
}


} //namespace
