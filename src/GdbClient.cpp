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

static const char * loadTempCommand = "$m%x,%x#";
static const char * storeTempCommand = "$M%x,%x:";
static const char TOKEN_CHECKSUM = '#';
static const char * TOKEN_ACK_SUCCESS = "+";
static const char TOKEN_START_PACKET = '$';
static const char * RESPONSE_OK = "$OK#";

bool ClientGDB::resetRemoteUnit(const ResetType type) const {

    (void)type;
    (void)loadTempCommand;
    return false;
}

static uint8_t getQtyBytes(const llvm_pass_arg sizeValBits ){
    switch (sizeValBits) {
    case 1:
        return 1;
    case 8:
        return 1;
    case 16:
        return 2;
    case 32:
        return 4;
    default:
        ADIN_LOG(_ERROR) << "Unrichable size of type: " << sizeValBits << "-bits";
        return 0;
    }
}

bool ClientGDB::store2RemoteAddr(const llvm_ocd_addr addr, const llvm_pass_arg value, const llvm_pass_arg sizeVal) const {

    const uint8_t size = getQtyBytes(sizeVal);

    if(size == 0)
        return false;

    const char *p = reinterpret_cast<const char *>(&value);

    return fastWrite2RemoteMem(addr, p, size);
}

bool ClientGDB::loadFromRemoteAddr(const llvm_ocd_addr addr, llvm_pass_arg & value, const llvm_pass_arg sizeVal) const {

    const uint8_t size = getQtyBytes(sizeVal);

    if(size == 0)
        return false;

    char *p = reinterpret_cast<char *>(&value);

    return fastLoadFromRemoteMem(addr, size, p);
}


static uint8_t  checkSumModulo256(const char* start, const char * end){
    size_t sum = 0;
    for(size_t i=0; (start+i) != end; i++){
        sum += static_cast<uint8_t>(*(start + i));
    }
    sum = sum % 256;

    return static_cast<uint8_t>(sum);
}

static bool sendAck(const char * token){
    return sendMessage2Server(token, 1);
}

#define SIZE_DATA 3

static void appendNumber8(char * packet, const uint8_t sum){
    char str[SIZE_DATA]={'\0'};
    snprintf(str, SIZE_DATA, "%02x", sum);
    strncpy(packet, str, 2);
}


static void appendSum(const char* start, const char * end, char * append_ptr){
    const uint8_t sum = checkSumModulo256(start, end);
    appendNumber8(append_ptr, sum);
    append_ptr[2] = '\0';
}


bool ClientGDB::fastWrite2RemoteMem(const uintptr_t addr, const char* sink, const size_t size) const {
    char str[SIZE_DATA]={'\0'};

    const size_t potential_size_buffer = strlen(storeTempCommand) + size*2
        + 8 //addr
        + 10 // size (string)
        + 2 //checksum
        + 1; //null terminated

    if(bufferSend.size() < potential_size_buffer){
        bufferSend.resize(potential_size_buffer);
    }

    char * packet = bufferSend.data();

    const int len = snprintf(packet, bufferSend.size(),
                             storeTempCommand,
                             addr, size);

    asser_1line(len > 0);

    char * data = packet + len;

    for (size_t i = 0; i < size; i++) {
        const uint8_t val = static_cast<uint8_t>(sink[i]);
        snprintf(str, SIZE_DATA, "%02x", val);

        data[2*i] = str[0];
        data[2*i + 1] = str[1];
    }

    size_t end = static_cast<size_t>(len) + size*2;
    packet[end] = '#';
    end++;

    appendSum(packet + 1, packet + end - 1, packet + end);

    const size_t size_packet = strlen(packet);

    size_t lenBufReceiv;
    commandSendAndGetResponse(packet, size_packet,
                              bufferReceiv, lenBufReceiv, TOKEN_CHECKSUM);

    const char * start = strchr(bufferReceiv.data(), TOKEN_START_PACKET);

    if((start == nullptr) ||
        (strncmp(start, RESPONSE_OK, strlen(RESPONSE_OK)) != 0))
    {
        const string resp(bufferReceiv.data(), lenBufReceiv);
        ADIN_LOG(_ERROR) << "GDB server error respose: " << resp;
        ADIN_LOG(_ERROR) << "len of response : " << lenBufReceiv;
        return false;
    }

    sendAck(TOKEN_ACK_SUCCESS);

    return true;
}


bool ClientGDB::fastLoadFromRemoteMem(const uintptr_t addr, const size_t size, char* dist) const {

    const size_t size_receive = size*2 + 1 /*start*/ + 3 /*sum*/;
    char buf[SIZE_DATA]={'\0'};

    char * packet = bufferSend.data();

    if(bufferReceiv.size() < size_receive){
        bufferReceiv.resize(size_receive);
    }

    const int len = snprintf(packet, bufferSend.size(),
                             loadTempCommand,
                             addr, size);

    appendSum(packet + 1, packet + len - 1, packet + len);

    size_t lenBufReceiv;
    commandSendAndGetResponse(packet, strlen(packet),
                              bufferReceiv, lenBufReceiv, TOKEN_CHECKSUM);

    const char * start = strchr(bufferReceiv.data(), TOKEN_START_PACKET);

    if((start == nullptr) ||
        (strlen(start) != size_receive))
    {
        const string resp(bufferReceiv.data(), lenBufReceiv);
        ADIN_LOG(_ERROR) << "GDB server error respose: "  << resp;
        ADIN_LOG(_ERROR) << "len of response : " << size_receive << " [ " << lenBufReceiv << " ]";
        return false;
    }

    const char * data = start + 1;

    for(size_t i = 0; i < size; i++){
        buf[0] = data[i*2];
        buf[1] = data[i*2+1];
        buf[2] = '\0';
        dist[i] = strtoul(buf, nullptr, 16) & 0xFF;
    }

    sendAck(TOKEN_ACK_SUCCESS);

    return true;
}

} //namespace
