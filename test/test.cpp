

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>
#include <assert.h>

#include "netwrapper.h"
#include "AddressInterceptPass.h"
#include "adin.h"
#include "logger.h"
#include "assertion.h"

#include "test/IrTest.h"


#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wextra"

using namespace adin;
using namespace std;

static bool callback(const char *__assertion, const char *__file,
               const unsigned int __line, const char *__function){
    std::cout << "test custom error callback : " << endl;
    std::cout << "$" << __file << "/" << __function << ":" << __line
              << ": " << __assertion << endl;
    return true;
}

llvm_pass_arg v = 0x87654321;

static const uint16_t PORT_TCL = 6666;
static const uint16_t PORT_GDB = 3333;

int main(int argc, char** argv)
{

    if(argc != 3){
        printf("test requare 2 arguments: host 32bit_hex_address\n");
        return -1;
    }

    int ret = 0;

    const string host(argv[1]);
    const uint32_t address = strtoul(argv[2], NULL, 16) & 0xFFFFFFFF;

    setVerboseLevel(_INFO);
    ADIN_LOG(_INFO) << "host: " << host;
    ADIN_LOG(_INFO) << "address: 0x" << hex<< address;
    setVerboseLevel(_ERROR);

    std::cout << "----------------------- Test OpenOCD client -----------------------" << endl;

    connect2OpenOCD(host, PORT_TCL);

    resetRemoteUnit(ResetType::__HALT);

    addInterceptAddress2Interval(address, address + 4*2);

    ret = irTest(reinterpret_cast<int*>(address));

    assert(ret == 0);

    const char * testMessage = "test message";

    const size_t _SIZE = strlen(testMessage);

    char dist[100] = {'\0'};

    fastWrite2RemoteMem(address, testMessage, _SIZE);

    fastLoadFromRemoteMem(address, 100, dist);

    ret = strncmp(testMessage, dist, _SIZE);

    assert(ret == 0);

    closeTCP();
    setErrorFunction(callback);

    std::cout << "Callback function:" << endl;

    fastWrite2RemoteMem(address,dist,1);

    setErrorFunction(nullptr);

    std::cout << "----------------------- Test RSP GDB client -----------------------" << endl;

    connect2Server(host, PORT_GDB, _GDB_SERVER);

    addInterceptAddress2Interval(address, address + 4*4);

    #define QQQ 33
    uint8_t q[QQQ];
    for(int i =0; i < QQQ; i++)
        q[i] = i;
    fastWrite2RemoteMem(0x20000000, (const char *)&q, 10);

    char  buf[QQQ] = {'@'};

    fastLoadFromRemoteMem(0x20000000, 10, buf);

    assert(std::strncmp((char*)q,buf,10) == 0);

    ret = irTest(reinterpret_cast<int*>(0x20000000));

    assert(ret == 0);

    closeTCP();
    setErrorFunction(callback);

    std::cout << "Callback function:" << endl;

    fastWrite2RemoteMem(address,dist,1);
}

