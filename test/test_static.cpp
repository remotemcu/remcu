

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
#include "AddressInterval.h"
#include "AddressFunction.h"

#include "test/IrTest.h"


#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wextra"
#pragma clang diagnostic ignored "-Wmacro-redefined"

using namespace remcu;
using namespace std;

static bool error = false;

static void callback(){
    cout << "callback()" << endl;
    error = true;
}

static const uint16_t PORT_TCL = 6666;
static const uint16_t PORT_GDB = 3333;

#define _STRING_ "123456789abc"

void assertErrorTest(uint32_t address){
    std::cout << "\n----------------------- Test Error -----------------------\n" << endl;

    setErrorSignalFunc(callback);
    assert(getErrorCout() == 0);
    assert(error == false);
    irTestSimple(reinterpret_cast<int*>(address));
    assert(getErrorCout() > 0);
    assert(error == true);
    error = false;
    clearErrorCount();

    setErrorSignalFunc(nullptr);
    irTestSimple(reinterpret_cast<int*>(address));
    assert(error == false);
    clearErrorCount();
}

void standartTestAddr(uint32_t address){

    int ret = irTest(reinterpret_cast<int*>(address));

    assert(ret == 0);

    #define _SIZE 30
    uint8_t testMessage[_SIZE];
    for(int i =0; i < _SIZE; i++)
        testMessage[i] = i;

    uint8_t dist[100] = {'\0'};

    store2addr(address, testMessage, _SIZE);

    loadFromAddr(address, 100, dist);

    ret = strncmp((char*)testMessage, (char*)dist, _SIZE);

    assert(store2mem(address, testMessage, _SIZE_ONE_MEMPCY-1) == false);
    assert(store2mem(address, testMessage, _SIZE_ONE_MEMPCY) == false);

    assert(ret == 0);

    disconnect();

    assertErrorTest(address);
}


int main(int argc, char** argv)
{

    const string dec = cryptor::create(_STRING_).decrypt();

    printf("dec %s [%d]\n", dec.c_str(), dec.size());

    printLogo();


    if(argc < 3){
        printf("test requare 2 arguments: host and verbose level\n");
        printf("optional 3-d arg: testOpenocd(bool)\n");
        return -1;
    }

    int ret = 0;

    const string host(argv[1]);
    const uint32_t address = 0x20000000;
    const LevelDebug level = static_cast<LevelDebug>(atoi(argv[2]) & 0xF);
    bool testOpenocd = true;


    printf("!!!! %d '%s'\n", argc, argv[3]);

    if(argc > 3){
        if(string(argv[3]).compare("no") == 0){
            testOpenocd = false;
        }
    }

    setVerboseLevel(__INFO);
    ADIN_LOG(__INFO) << "host: " << host;
    ADIN_LOG(__INFO) << "address: 0x" << hex << address;
    ADIN_LOG(__INFO) << "Verbose Level: " << level;
    setVerboseLevel(level);

    assertErrorTest(address);

    setConfig("TEST_CONFIG_MEM");

    if(testOpenocd){
        std::cout << "\n----------------------- Test OpenOCD client -----------------------\n" << endl;

        std::cout << getVersion() << endl;

        connect2OpenOCD(host, PORT_TCL);

        resetRemoteUnit(ResetType::__HALT);

        standartTestAddr(address);
    }

    std::cout << "\n----------------------- Test RSP GDB client -----------------------\n" << endl;

    connect2GDB(host, PORT_GDB);

    resetRemoteUnit(ResetType::__HALT);

    standartTestAddr(address);

    std::cout << "\n----------------------- SUCCESS TEST -----------------------\n" << endl;

}

