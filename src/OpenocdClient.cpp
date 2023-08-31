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

#define COMMAND_TOKEN '\x1a'

static const char * loadTempCommandOld = "ocd_md%c 0x%lX\x1a";
static const char * loadTempCommandNew = "md%c 0x%lX\x1a";
static const char * loadTempCommand = loadTempCommandOld;

static const char * storeTempCommand = "mw%c 0x%lX 0x%lX\x1a";

static const char * loadArrayTempCommandOld = "ocd_mdb 0x%lX %d\x1a";
static const char * loadArrayTempCommandNew = "mdb 0x%lX %d\x1a";
static const char * loadArrayTempCommand = loadArrayTempCommandOld;

enum SizeOperation_t {
    BYTE = 'b', HALF_WORD = 'h', WORD = 'w'
};

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
        ADIN_LOG(__ERROR) << _S_("Unknown type raz: ") << sizeVal;
        return false;
    }

    return true;
}

// todo: check len 
static bool parseValue(vector<char> & buffer, llvm_value_type & value){

    char * p = buffer.data();

    if(strtok(p,":") == nullptr)
        return false;
    const char * valp = strtok(nullptr,": ");

    if(valp == nullptr){
        return false;
    }

    value = strtoul(valp, nullptr, 16);

    return true;
}



bool ClientOpenOCD::ping() const {
    static const char _RESPONSE = '*';
    string pingCommand(_S_("ocd_echo "));
    pingCommand.push_back(_RESPONSE);
    pingCommand.push_back('\x1a');
    size_t lenReceiv;
    assert_1message(commandSendAndGetResponse(pingCommand.c_str(), pingCommand.size(),
                                              bufferReceiv, lenReceiv, COMMAND_TOKEN),
                    "Server disconnect already");
    return true;
}

// GNU MCU Eclipse OpenOCD, 64-bitOpen On-Chip Debugger 0.10.0+dev-00593-g23ad80df4 (2019-04-22-20:18)
// https://github.com/ilg-archived/openocd/releases/tag/v0.10.0-12-20190422
// xPack Open On-Chip Debugger 0.12.0-01004-g9ea7f3d64-dirty (2023-01-30-15:03)
// https://github.com/xpack-dev-tools/openocd-xpack/releases/tag/v0.12.0-1/
const string ver1 = string("GNU MCU Eclipse OpenOCD, 64-bitOpen On-Chip Debugger 0.10.0+dev-00593-g23ad80df4");
const string ver2 = string("xPack Open On-Chip Debugger 0.12.0-01004-g9ea7f3d64");


bool ClientOpenOCD::provisioning() const {
    string version;
    if(get_version(version) == false)
        return false;

    ADIN_LOG(__INFO) << "openocd version: '" << version << "'";

    // to do: check array versions in the loop
    if(version.rfind(ver1) == 0){
        ADIN_LOG(__INFO) << "supported OpenOCD version. Use Old command version";
        loadTempCommand = loadTempCommandOld;
        loadArrayTempCommand = loadArrayTempCommandOld;
        return true;
    }
    if(version.rfind(ver2) == 0){
        ADIN_LOG(__INFO) << "supported OpenOCD version. Use New command version";
        loadTempCommand = loadTempCommandNew;
        loadArrayTempCommand = loadArrayTempCommandNew;
        return true;
    }

    ADIN_LOG(__ERROR) << "\n\n ********************* Error ********************\n\n";

    ADIN_LOG(__ERROR) << "The OpenOCD '" << version << "' version";
    ADIN_LOG(__ERROR) << "you are currently using is not supported.";
    ADIN_LOG(__ERROR) << "To ensure optimal compatibility and functionality,";
    ADIN_LOG(__ERROR) << "we recommend exclusively utilizing version downloadable from here:"; 
    ADIN_LOG(__ERROR) << "https://github.com/ilg-archived/openocd/releases/tag/v0.10.0-12-20190422";
    ADIN_LOG(__ERROR) << "or version 0.12 (available from here:";
    ADIN_LOG(__ERROR) << "https://github.com/xpack-dev-tools/openocd-xpack/releases/tag/v0.12.0-1/";
    ADIN_LOG(__ERROR) << "If you are certain that your specific use case necessitates a different version,";
    ADIN_LOG(__ERROR) << "please be advised that modifications to this file will be required";

    ADIN_LOG(__ERROR) << "\n\n ^^^^^^^^^^^^^^^^^^^^ Error ^^^^^^^^^^^^^^^^^^^^\n\n";

    return false;
}


bool ClientOpenOCD::get_version(string & version) const {
    string versionCommand("version");
    versionCommand.push_back('\x1a');
    size_t lenReceiv;
    assert_1message(commandSendAndGetResponse(versionCommand.c_str(), versionCommand.size(),
                                              bufferReceiv, lenReceiv, COMMAND_TOKEN),
                    "Server disconnect already");

    version = string(bufferReceiv.data(), lenReceiv);

    return true;    
}


bool ClientOpenOCD::resetRemoteUnit(const ResetType type) const {
    string reset_message(_S_("reset "));

    switch (type) {
    case ResetType::__RUN:
        reset_message.append(_S_("run"));
        break;
    case ResetType::__HALT:
        reset_message.append(_S_("halt"));
        break;
    default:
        ADIN_LOG(__ERROR) << _S_("Unknown reset type: ") << type;
        return false;
    }

    reset_message.push_back('\x1a');

    size_t lenReceiv;
    return commandSendAndGetResponse(reset_message.c_str(), reset_message.size(), bufferReceiv, lenReceiv, COMMAND_TOKEN);
}

bool ClientOpenOCD::store2RemoteAddr(const llvm_ocd_addr addr, const llvm_value_type value, const llvm_pass_arg sizeVal) const {

    llvm_pass_arg mask = 0;
    char sizeOp = 0;

    assert_1message(getMaskAndSize(sizeVal, mask, sizeOp), "attribute of value - error");

    const llvm_value_type sendValue = value & mask;

    const int len = snprintf(bufferSend.data(), bufferSend.size(),
                    storeTempCommand,
                             sizeOp, addr, sendValue);

    size_t lenReceiv;
    return commandSendAndGetResponse(bufferSend.data(), static_cast<size_t>(len), bufferReceiv, lenReceiv, COMMAND_TOKEN);
}

bool ClientOpenOCD::loadFromRemoteAddr(const llvm_ocd_addr addr, llvm_value_type & value, const llvm_pass_arg sizeVal) const {

    llvm_pass_arg mask = 0;
    char sizeOp = 0;

    assert_1message(getMaskAndSize(sizeVal, mask, sizeOp), "attribute of value - error");


    const int len = snprintf(bufferSend.data(), bufferSend.size(),
                             loadTempCommand,
                             sizeOp, addr);

    size_t lenReceiv = 0;
    commandSendAndGetResponse(bufferSend.data(), static_cast<size_t>(len), bufferReceiv, lenReceiv, COMMAND_TOKEN);

    assert_printf(parseValue(bufferReceiv, value), "can't parse answer of server: [%d] %s\n", lenReceiv, bufferReceiv.data());

    return true;
}

// todo: DEPRECATED! use 'write_memory' not 'array2mem'
static const char * array2memTemplate = "array unset MASS; array set MASS { %s }; array2mem MASS %d 0x%x %d\x1a";
static const char * arrayFirstPart = "array unset MASS; array set MASS {";
static const char * arraySecondPart = " }; array2mem MASS 8 0x%x %d\x1a";
static const char * elementTemplate = " %d 0x%x";
const size_t sizeBuf = 200;
static char buf[sizeBuf];

static vector<char> arrayBuffer(100);

bool ClientOpenOCD::arrayWrite2RemoteMem(const uintptr_t addr, const uint8_t*  sink, const size_t size) const {

    const size_t potential_array_size = size*6;
    const size_t _ADD = 100;

    int len = -1;
    size_t usedBufferBytes = 0;

    if(arrayBuffer.size() < potential_array_size){
        arrayBuffer.resize(potential_array_size);
    }

    for(size_t i =0; i < size; i++){
        len = snprintf(buf, sizeBuf, elementTemplate, i, sink[i]);
        assert_1message(len > 0, "can't create packet for server");
        const size_t future_size = usedBufferBytes + static_cast<size_t>(len);
        if(arrayBuffer.size() <= future_size){
            arrayBuffer.resize(future_size + _ADD);
        }
        strncpy(arrayBuffer.data() + usedBufferBytes, buf, static_cast<size_t>(len));
        usedBufferBytes += static_cast<size_t>(len);
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
    assert_1message(len > 0, "error package for server");
    strncpy(bufferSend.data() + pos, buf, static_cast<size_t>(len));

    pos += static_cast<size_t>(len);

    size_t lenBufReceiv;
    return commandSendAndGetResponse(bufferSend.data(), pos, bufferReceiv, lenBufReceiv, COMMAND_TOKEN);
}

static const size_t qty_value_in_row = 32;
static const size_t qty_service_bytes = 12;
static const size_t _GAP = 100;
static const char _SEPARATOR_LOAD = ' ';

bool ClientOpenOCD::arrayLoadFromRemoteMem(const uintptr_t addr, const size_t size, uint8_t*  dist) const {

    const size_t potential_size_receive = size*3 +
        (size/qty_value_in_row)*qty_service_bytes + _GAP;

    if(bufferReceiv.size() < potential_size_receive){
        bufferReceiv.resize(potential_size_receive + _GAP);
    }

    const int len = snprintf(bufferSend.data(), bufferSend.size(),
                             loadArrayTempCommand,
                             addr, size);

    size_t lenBuf = 0;
    assert_1message(commandSendAndGetResponse(bufferSend.data(), static_cast<size_t>(len), bufferReceiv, lenBuf, COMMAND_TOKEN),
                    "Server error^");

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
        if(isxdigit(buf[0]) && isxdigit(buf[1])){
            dist[i] = strtoul(buf, nullptr, 16) & 0xFF;
        } else {
            ADIN_PRINTF(__ERROR, "wrong number format from server. s: %s\n", buf);
            ADIN_PRINTF(__ERROR, "full s: %s\n", point);
            return false;
        }
    }

    return true;
}


} //namespace

#pragma clang diagnostic pop

