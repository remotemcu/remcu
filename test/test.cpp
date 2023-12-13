

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>
#include <assert.h>

#include "Ir.h"
#include "remcu.h"


#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wextra"
#pragma clang diagnostic ignored "-Wmacro-redefined"

using namespace std;

static bool error = false;


static const uint16_t PORT_TCL = 6666;
static const uint16_t PORT_GDB = 3333;
static const int DEFAULT_TIMEOUT = 1;

#define _STRING_ "123456789abc"

void assertErrorTest(uint32_t address){
    std::cout << "\n----------------------- Test Error -----------------------\n" << endl;

    assert(remcu_getErrorCount() == 0);
    assert(error == false);
    simpleTest(reinterpret_cast<int*>(address));
    assert(remcu_getErrorCount() > 0);
    //assert(error == true);
    error = false;
    remcu_clearErrorCount();

    //remcu_setErrorSignalFunc(NULL);
    simpleTest(reinterpret_cast<int*>(address));
    assert(error == false);
    remcu_clearErrorCount();
}

void standartTestAddr(uintptr_t address){
    int sanbox[111] = {0};
    int ret = 0;

    assert(remcu_debuggerTest() == NULL);

    assert(remcu_isConnected());

    const char* error = remcu_debuggerTest();

    assert(error == NULL);

    #define _SIZE 30
    uint8_t testMessage[_SIZE];
    const size_t qty_write = _SIZE - 1;
    for(int i =0; i < qty_write; i++)
        testMessage[i] = '0' + i;

    testMessage[_SIZE - 1] = '\0';

    uint8_t dist[100] = {'\0'};

    assert(remcu_store2mem(address, testMessage, _SIZE));

    assert(remcu_loadFrMem(address, _SIZE, dist));

    ret = strncmp((char*)testMessage, (char*)dist, _SIZE);

    assert(ret == 0);

    assert(remcu_store2mem(address, testMessage, MAX_MEM_OPERATION_SIZE) == true);
    assert(remcu_store2mem(address, testMessage, MAX_MEM_OPERATION_SIZE + 1) == false);

    testStoreLoadAllType(address);

    assert(remcu_store2mem(address, testMessage, MIN_MEM_OPERATION_SIZE) == true);
    assert(remcu_store2mem(address, testMessage, MIN_MEM_OPERATION_SIZE - 1) == false);

    remcu_disconnect();

    assertErrorTest(address);

    assert(remcu_isConnected() == false);
}

int main(int argc, char** argv)
{
#define _SIZE_VERSION 100

    assert(remcu_getVersion());
    std::cout << "version : " << remcu_getVersion() << endl;

    if(argc <= 4){
        printf("test requare 2 arguments: host, port of TCL(OpenOCD), port GDB, test hex address, verbose\n");
        printf("if port 0 then no test this port\n");
        return -1;
    }

    int ret = 0;

    const string host(argv[1]);
    const uint16_t portTCL = (atoi(argv[2]) & 0xFFFF);
    const uint16_t portGDB = (atoi(argv[3]) & 0xFFFF);

    const uintptr_t address = stoi( argv[4], 0, 16 ); //0x20000000;
    const LevelDebug level = static_cast<LevelDebug>(atoi(argv[5]) & 0xF);

    printf("argc: %d \n", argc);


    cout << "host: " << host << endl;
    cout << "portTCL: " << portTCL << endl;
    cout << "portGDB: " << portGDB << endl;
    cout << "address: 0x" << hex << address << endl;
    cout << "Verbose Level: " << level << endl;
    remcu_setVerboseLevel(level);

    assertErrorTest(address);

    assert(remcu_isConnected() == false);

    if(portTCL != 0){
        std::cout << "\n----------------------- Test OpenOCD client -----------------------\n" << endl;

        assert(remcu_connect2OpenOCD(host.c_str(), portTCL, DEFAULT_TIMEOUT));

        assert(remcu_resetRemoteUnit(__RUN));

        assert(remcu_resetRemoteUnit(__HALT));

        standartTestAddr(address);
    }

    if(portGDB != 0){

        std::cout << "\n----------------------- Test RSP GDB client -----------------------\n" << endl;

        assert(remcu_connect2GDB(host.c_str(), portGDB, DEFAULT_TIMEOUT));

        assert(remcu_resetRemoteUnit(__HALT));

        assert(remcu_resetRemoteUnit(__RUN) == false);

        standartTestAddr(address);
    } else {
        cout << "pass Test RSP GDB client" << endl;
    }

    std::cout << "\n----------------------- SUCCESS ALL TEST -----------------------\n" << endl;

    
}

