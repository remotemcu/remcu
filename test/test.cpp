

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>
#include <assert.h>

#include "OpenocdClient.h"
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
                unsigned int __line, const char *__function){
    cout << "test custom error callback " << endl;
    return true;
}

llvm_pass_arg v = 0x87654321;

int main(int argc, char** argv)
{
    if(argc != 3){
        assert("test requare 3 arguments: host port 32bit_hex_address");
    }


    const string host(argv[1]);
    const uint16_t port = atoi(argv[2]) & 0xFFFF;
    const uint32_t address = strtoul(argv[3], NULL, 16) & 0xFFFFFFFF;

    setVerboseLevel(_INFO);
    ADIN_LOG(_INFO) << "host: " << host;
    ADIN_LOG(_INFO) << "port: " << port;
    ADIN_LOG(_INFO) << "address: 0x" << hex<< address;
    setVerboseLevel(_ERROR);

    connect2OpenOCD(host, port);

    setVerboseLevel(_ALL_LOG);
    asser_1line("print line");
    setVerboseLevel(_ERROR);
    setErrorFunction(callback);

    addInterceptAddress2Interval(address, address + 4*2);

    int ret = irTest(reinterpret_cast<int*>(address));

    assert(ret == 0);

    const char * testMessage = "test message";

    const size_t _SIZE = strlen(testMessage);

    char dist[100] = {'\0'};


    fastWrite2RemoteMem(address, testMessage, _SIZE);

    fastLoadFromRemoteMem(address, _SIZE + 100, dist);

    ret = strncmp(testMessage, dist, _SIZE);

    assert(ret == 0);

    asser_1line(false);
}
