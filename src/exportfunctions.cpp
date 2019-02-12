
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>
#include <assert.h>
#include <vector>

#include "AddressInterceptPass.h"
#include "logger.h"
#include "assertion.h"
#include "client.h"
#include "netwrapper.h"
#include "obusfaction.h"

using namespace  std;

namespace adin {

bool connect2Server(const std::string host, const uint16_t port,
                    const ServerType server = _DUMMY_SERVVER,
                    const int timeout_sec = _DEFAULT_TIMEOUT_SEC);

bool addInterceptAddress2Interval(const llvm_ocd_addr start, const llvm_ocd_addr end);

void clearAllInterceptAddressInterval();

void clearMCU(){
    clearAllInterceptAddressInterval();
}


bool connect2OpenOCD(const std::string host, const uint16_t port,
                     const int timeout_sec){
    return connect2Server(host, port, _OPENOCD_SERVER, timeout_sec);
}

bool connect2GDB(const std::string host, const uint16_t port,
                 const int timeout_sec){
    return connect2Server(host, port, _GDB_SERVER, timeout_sec);
}

bool disconnect(){
    return closeTCP();
}

static string targetMCU("unknown");

std::string getCurrentMCU(){
    return targetMCU;
}

bool setMCU(const std::string target){

    if(target.compare("STM32F10X_MD") == 0){
        addInterceptAddress2Interval(0x20000000, 0x20000000 + 20*1024);
        targetMCU.assign(target);
    } else {
        return false;
    }
    return true;
}


}
