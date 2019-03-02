#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>
#include <assert.h>

#include "test/IrTest.h"

using namespace std;

static bool error = false;

static void callback(){
    cout << "callback()" << endl;
    error = true;
}

#ifdef NDEBUG
    #define assert
#endif

static const uint16_t PORT_TCL = 6666;
static const uint16_t PORT_GDB = 3333;

#define _STRING_ "123456789abc"

void assertErrorTest(uint32_t address){
    std::cout << "\n----------------------- Test Error -----------------------\n" << endl;

    remcu_setErrorSignalFunc(callback);
    assert(remcu_getErrorCout() == 0);
    assert(error == false);
    irTestSimple(reinterpret_cast<int*>(address));
    assert(remcu_getErrorCout() > 0);
    assert(error == true);
    error = false;
    remcu_clearErrorCount();

    remcu_setErrorSignalFunc(nullptr);
    irTestSimple(reinterpret_cast<int*>(address));
    assert(error == false);
    remcu_clearErrorCount();
}

void standartTestAddr(uint32_t address){

    assert(remcu_is_connected());

    int ret = irTest(reinterpret_cast<int*>(address));

    assert(ret == 0);

#define _SIZE 30
    uint8_t testMessage[_SIZE];
    for(int i =0; i < _SIZE; i++)
        testMessage[i] = i;

    uint8_t dist[100] = {'\0'};

    remcu_store2mem(address, testMessage, _SIZE);

    remcu_loadFrMem(address, 100, dist);

    ret = strncmp((char*)testMessage, (char*)dist, _SIZE);

    assert(remcu_store2mem(address, testMessage, _SIZE_ONE_MEMPCY-1) == true);
    assert(remcu_store2mem(address, testMessage, _SIZE_ONE_MEMPCY) == false);

    assert(ret == 0);

    remcu_disconnect();

    assertErrorTest(address);

    assert(remcu_is_connected() == false);
}


int main(int argc, char** argv)
{

#define _SIZE_VERSION 100
    char version[_SIZE_VERSION] = {'\0'};
    size_t len = _SIZE_VERSION;
    assert(remcu_getVersion(version, len));
    std::cout << "version : " << version << endl;

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

    printf("argc: %d '%s'\n", argc, argv[3]);

    if(argc > 3){
        if(string(argv[3]).compare("no") == 0){
            testOpenocd = false;
        }
    }

    cout << "host: " << host << endl;
    cout << "address: 0x" << hex << address << endl;
    cout << "Verbose Level: " << level << endl;
    remcu_setVerboseLevel(level);

    assertErrorTest(address);

    assert(remcu_setConfig("ERROR") == false);

    assert(remcu_setConfig("TEST_CONFIG_MEM"));

    assert(remcu_is_connected() == false);

    if(testOpenocd){
        std::cout << "\n----------------------- Test OpenOCD client -----------------------\n" << endl;

        assert(remcu_connect2OpenOCD(host.c_str(), PORT_TCL));

        assert(remcu_resetRemoteUnit(ResetType::__HALT));

        standartTestAddr(address);
    }

    std::cout << "\n----------------------- Test RSP GDB client -----------------------\n" << endl;

    assert(remcu_connect2GDB(host.c_str(), PORT_GDB));

    assert(remcu_resetRemoteUnit(ResetType::__HALT));

    assert(remcu_resetRemoteUnit(ResetType::__INIT) == false);

    standartTestAddr(address);

    std::cout << "\n----------------------- SUCCESS TEST -----------------------\n" << endl;

}
