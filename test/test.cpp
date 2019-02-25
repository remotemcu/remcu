

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>
#include <assert.h>

#include "netwrapper.h"
#include "AddressInterceptPass.h"
#include "exports.h"
#include "logger.h"
#include "assertion.h"
#include "obusfaction.h"
#include "logofun.h"

#include "test/IrTest.h"


#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wextra"
#pragma clang diagnostic ignored "-Wmacro-redefined"

using namespace remcu;
using namespace std;

static bool error = false;

static void callback(){
    error = true;
}

static const uint16_t PORT_TCL = 6666;
static const uint16_t PORT_GDB = 3333;

#define _STRING_ "123456789abc"

void assertErrorTest(uint32_t address){
    std::cout << "----------------------- Test Error -----------------------" << endl;

    assert(getErrorCout() == 0);
    assert(error == false);
    irTestSimple(reinterpret_cast<int*>(address));
    assert(getErrorCout() > 0);
    assert(error == true);
    error = false;
    clearErrorCount();
}


int main(int argc, char** argv)
{

    const string dec = cryptor::create(_STRING_).decrypt();

    printf("dec %s [%d]\n", dec.c_str(), dec.size());

    printLogo();


    if(argc != 3){
        printf("test requare 2 arguments: host and verbose level\n");
        return -1;
    }

    int ret = 0;

    const string host(argv[1]);
    const uint32_t address = 0x20000000;
    const LevelDebug level = static_cast<LevelDebug>(atoi(argv[2]) & 0xF);

    setVerboseLevel(__INFO);
    ADIN_LOG(__INFO) << "host: " << host;
    ADIN_LOG(__INFO) << "address: 0x" << hex << address;
    ADIN_LOG(__INFO) << "Verbose Level: " << level;
    setVerboseLevel(level);

    assertErrorTest(address);

    std::cout << "----------------------- Test OpenOCD client -----------------------" << endl;

    std::cout << getVersion() << endl;

    connect2OpenOCD(host, PORT_TCL);

    resetRemoteUnit(ResetType::__HALT);

    setConfig("STM32F10X_MD");

    ret = irTest(reinterpret_cast<int*>(address));

    assert(ret == 0);

    const char * testMessage = "test message";

    const size_t _SIZE = strlen(testMessage);

    char dist[100] = {'\0'};

    arrayWrite2RemoteMem(address, testMessage, _SIZE);

    arrayLoadFromRemoteMem(address, 100, dist);

    ret = strncmp(testMessage, dist, _SIZE);

    assert(ret == 0);

    disconnect();

    assertErrorTest(address);

    std::cout << "----------------------- Test RSP GDB client -----------------------" << endl;

    connect2GDB(host, PORT_GDB);

    resetRemoteUnit(ResetType::__HALT);

    setConfig("STM32F10X_MD");

    #define _SIZE 33
    uint8_t test_msg[_SIZE];
    for(int i =0; i < _SIZE; i++)
        test_msg[i] = i;
    arrayWrite2RemoteMem(address, (const char *)&test_msg, 10);

    char  buf[_SIZE] = {'@'};

    arrayLoadFromRemoteMem(address, 10, buf);

    assert(std::strncmp((char*)test_msg,buf,10) == 0);

    ret = irTest(reinterpret_cast<int*>(0x20000000));

    assert(ret == 0);

    disconnect();

    assertErrorTest(address);
}

