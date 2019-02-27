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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"

using namespace std;


namespace remcu {

static const char * loadTempCommand = "$m%x,%x#";
static const char * storeTempCommand = "$M%x,%x:";
static const char TOKEN_CHECKSUM = '#';
static const char * TOKEN_ACK_SUCCESS = "+";
static const char * TOKEN_ACK_FAILED = "-";
static const char TOKEN_START_PACKET = '$';
static const char * RESPONSE_OK = "$OK#";

static bool sendAck(const char * token){
    return sendMessage2Server(token, 1);
}

bool ClientGDB::resetRemoteUnit(const ResetType type) const {

    if(ResetType::__HALT != type){
        ADIN_LOG(__ERROR) << "GDB supports only halt reset ";
        return false;
    }

    static const char * RESET_COMMAND = "$R#52";
    const size_t size = strlen(RESET_COMMAND);

    size_t lenReceiv;
    if(commandSendAndGetResponse(RESET_COMMAND, size,
                                  bufferReceiv, lenReceiv, *TOKEN_ACK_SUCCESS) == false){
        ADIN_PRINTF(__INFO, "wait OK\n", 0);
    }

    if(readBeforeToken(bufferReceiv, lenReceiv, TOKEN_CHECKSUM) == false){
        ADIN_PRINTF(__INFO, "failed OK .. continue \n", 0);
    }

    assert_1message(sendAck(TOKEN_ACK_SUCCESS), "can't send ACK");

    return true;
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
        ADIN_LOG(__ERROR) << "Unrichable size of type: " << sizeValBits << "-bits";
        return 0;
    }
}

bool ClientGDB::store2RemoteAddr(const llvm_ocd_addr addr, const llvm_value_type value, const llvm_pass_arg sizeVal) const {

    const uint8_t size = getQtyBytes(sizeVal);

    if(size == 0)
        return false;

    const uint8_t *p = reinterpret_cast<const uint8_t *>(&value);

    return this->arrayWrite2RemoteMem(addr, p, size);
}

bool ClientGDB::loadFromRemoteAddr(const llvm_ocd_addr addr, llvm_value_type & value, const llvm_pass_arg sizeVal) const {

    const uint8_t size = getQtyBytes(sizeVal);

    if(size == 0)
        return false;

    uint8_t *p = reinterpret_cast<uint8_t *>(&value);

    return this->arrayLoadFromRemoteMem(addr, size, p);
}


static uint8_t  checkSumModulo256(const char* start, const char * end){
    size_t sum = 0;
    for(size_t i=0; (start+i) != end; i++){
        sum += static_cast<uint8_t>(*(start + i));
    }
    sum = sum % 256;

    return static_cast<uint8_t>(sum);
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


bool ClientGDB::arrayWrite2RemoteMem(const uintptr_t addr, const uint8_t*  sink, const size_t size) const {
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

    assert_1message(len > 0, "can't create packet for server");

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
    assert_1message(commandSendAndGetResponse(packet, size_packet,
                              bufferReceiv, lenBufReceiv, TOKEN_CHECKSUM),
    "Server error^");

    const char * start = strchr(bufferReceiv.data(), TOKEN_START_PACKET);

    if((start == nullptr) ||
        (strncmp(start, RESPONSE_OK, strlen(RESPONSE_OK)) != 0))
    {
        const string resp(bufferReceiv.data(), lenBufReceiv);
        ADIN_LOG(__ERROR) << "GDB server error respose: " << resp;
        ADIN_LOG(__ERROR) << "len of response : " << lenBufReceiv;
        return false;
    }

    assert_1message(sendAck(TOKEN_ACK_SUCCESS), "can't send ACK");

    return true;
}


bool ClientGDB::arrayLoadFromRemoteMem(const uintptr_t addr, const size_t size, uint8_t*  dist) const {

    const size_t size_receive = size*2 + 1 /*start*/ + 3 /*sum*/;
    char buf[SIZE_DATA]={'\0'};

    char * packet = bufferSend.data();

    if(bufferReceiv.size() < size_receive){
        bufferReceiv.resize(size_receive);
    }

    const int len = snprintf(packet, bufferSend.size(),
                             loadTempCommand,
                             addr, size);

    assert_1message(len > 0, "can't create packet for server");

    appendSum(packet + 1, packet + len - 1, packet + len);

    size_t lenBufReceiv;
    assert_1message(commandSendAndGetResponse(packet, strlen(packet),
                                              bufferReceiv, lenBufReceiv, TOKEN_CHECKSUM)
                        , "Server error^");

    const char * start = strchr(bufferReceiv.data(), TOKEN_START_PACKET);

    if((start == nullptr) ||
        (strlen(start) < size_receive))
    {
        const string resp(bufferReceiv.data(), lenBufReceiv);
        ADIN_LOG(__ERROR) << "GDB server error respose: "  << resp;
        ADIN_LOG(__ERROR) << "len of response : " << size_receive << " [ " << lenBufReceiv << " ]";
        return false;
    }

    const char * data = start + 1;

    for(size_t i = 0; i < size; i++){
        buf[0] = data[i*2];
        buf[1] = data[i*2+1];
        buf[2] = '\0';
        if(isxdigit(buf[0]) && isxdigit(buf[1])){
            dist[i] = strtoul(buf, nullptr, 16) & 0xFF;
        } else {
            ADIN_PRINTF(__ERROR, "wrong number format from server. s: %s\n", buf);
            ADIN_PRINTF(__ERROR, "full s: %s\n", start);
            sendAck(TOKEN_ACK_FAILED);
            return false;
        }
    }

    assert_1message(sendAck(TOKEN_ACK_SUCCESS), "can't send ACK");

    return true;
}

} //namespace

#pragma clang diagnostic pop
