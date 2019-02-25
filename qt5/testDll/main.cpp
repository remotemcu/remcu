#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>
#include <assert.h>

#include <remcu.h>
#include <IrTest.h>

using namespace std;
using namespace remcu;


static const uint16_t PORT_TCL = 6666;
static const uint16_t PORT_GDB = 3333;

#define _STRING_ "123456789abc"

int main()
{
    cout << "Hello World!" << endl;

    int ret = 0;

    const string host("localhost");
    const uint32_t address = 0x20000000;
    const LevelDebug level = static_cast<LevelDebug>(4);

    setVerboseLevel(level);

    std::cout << "----------------------- Test OpenOCD client -----------------------" << endl;

    std::cout << getVersion() << endl;

    connect2OpenOCD(host, PORT_TCL);

    resetRemoteUnit(ResetType::__HALT);

    setConfig("TEST_CONFIG_MEM");

    ret = irTest(reinterpret_cast<int*>(address));

    assert(ret == 0);

    const char * testMessage = "test message";

    const size_t _SIZE = strlen(testMessage);

    char dist[100] = {'\0'};

    remote_memcpy(address, testMessage, _SIZE);

    local_memcpy(address, 100, dist);

    ret = strncmp(testMessage, dist, _SIZE);

    assert(ret == 0);

    disconnect();

    std::cout << "Errors:" << endl;

    remote_memcpy(address,dist,1);
    remote_memcpy(address,nullptr,1);
    local_memcpy(address, 10, nullptr);

    std::cout << "----------------------- Test RSP GDB client -----------------------" << endl;

    connect2GDB(host, PORT_GDB);

    resetRemoteUnit(ResetType::__HALT);

    setConfig("TEST_CONFIG_MEM");

#define _SIZE 33
    uint8_t test_msg[_SIZE];
    for(int i =0; i < _SIZE; i++)
        test_msg[i] = i;
    remote_memcpy(address, (const char *)&test_msg, 10);

    char  buf[_SIZE] = {'@'};

    local_memcpy(address, 10, buf);

    assert(std::strncmp((char*)test_msg,buf,10) == 0);

    ret = irTest(reinterpret_cast<int*>(0x20000000));

    assert(ret == 0);

    disconnect();
    std::cout << "Errors:" << endl;

    remote_memcpy(address,dist,1);
    remote_memcpy(address,nullptr,1);
    local_memcpy(address, 10, nullptr);
    return 0;
}
