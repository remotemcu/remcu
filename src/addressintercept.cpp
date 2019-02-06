
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


using namespace std;

namespace adin {

typedef struct {
    llvm_ocd_addr startAddr;
    llvm_ocd_addr endAddr;
} AddrInterval;

static vector<AddrInterval> intervals;

static ClientDummy dummy;
static ClientOpenOCD openocd;
static ClientGDB gdb;

static ClientBase * client = static_cast<ClientBase*>(&dummy);

void addInterceptAddress2Interval(const llvm_ocd_addr start, const llvm_ocd_addr end){
    AddrInterval st = {start, end};
    intervals.push_back(st);
}

void clearAllInterceptAddressInterval(){
    intervals.clear();
}

static bool isEntryHalfInterval(const llvm_ocd_addr addr){
    for( const AddrInterval & i : intervals ){
        bool isEntry = addr >= i.startAddr;
        isEntry &= addr < i.endAddr;
        if(isEntry)
            return true;
    }
    return false;
}

bool connect2Server(const std::string host, const uint16_t port, const ServerType server,
                    const bool logo, const int timeout_sec){

    client->close();

    if(server == _DUMMY_SERVVER){
        client = static_cast<ClientBase*>(&dummy);
    } else if (server == _OPENOCD_SERVER) {
        client = static_cast<ClientBase*>(&openocd);
    } else if(server == _GDB_SERVER) {
        client = static_cast<ClientBase*>(&gdb);
        //assert(!"GDB client not implementet");
    } else {
        ADIN_LOG(__ERROR) << "unknown client, please choice _OPENOCD_SERVER or _GDB_SERVER";
        return false;
    }

    const bool success = client->connect(host,port, timeout_sec);

    if(success && logo){
        ADIN_LOG(__INFO) << "logo!!";
    }

    if(success == false){
        ADIN_LOG(__ERROR) << "Connecting failed!";
        ADIN_LOG(__ERROR) << "Please check server and try again...";
    }

    return success;
}

bool resetRemoteUnit(const ResetType type){
    assert_1message(client->resetRemoteUnit(type), "can't reset remote unit");
    return true;
}

static inline llvm_value_type loadLocalReturnValue(const llvm_ocd_addr pointer, const llvm_pass_arg TypeSizeArg, const llvm_pass_arg DECL_UNUSED AlignmentArg){

    llvm_value_type ret = 0;
    switch (TypeSizeArg) {
    case 1:
        ret = (*reinterpret_cast<uint8_t*>(pointer)) & 0x1;
        break;
    case 8:
        ret = (*reinterpret_cast<uint8_t*>(pointer)) & 0xFF;
        break;
    case 16:
        ret = (*reinterpret_cast<uint16_t*>(pointer)) & 0xFFFF;
        break;
    case 32:
        ret = (*reinterpret_cast<uint32_t*>(pointer)) & 0xFFFFFFFF;
        break;
    case 64:
        ret = (*reinterpret_cast<uint64_t*>(pointer));
        break;
    default:
        ADIN_LOG(__ERROR) << "Unknown size of type: " << TypeSizeArg;
        ADIN_PRINTF(__ERROR, "at the pointer: %p\n", pointer);
        break;
    }

    return ret;
}


static inline bool store(const llvm_ocd_addr pointer, const llvm_pass_arg value, const llvm_pass_arg TypeSizeArg, const llvm_pass_arg DECL_UNUSED AlignmentArg)
{
    if(isEntryHalfInterval(pointer) == false){
        return true;
    }

    assert_printf(client->store2RemoteAddr(pointer, value, TypeSizeArg),
                  "Can't write value to address: %p, size: %d\n", pointer, TypeSizeArg);
    return true;
}

static inline llvm_value_type load(const llvm_ocd_addr pointer, const llvm_pass_arg TypeSizeArg, const  llvm_pass_arg AlignmentArg)
{
    if(isEntryHalfInterval(pointer) == false)
        return loadLocalReturnValue(pointer, TypeSizeArg, AlignmentArg);

    llvm_pass_arg  value;
    if(client->loadFromRemoteAddr(pointer, value, TypeSizeArg) == false){
        value = 0;
        ADIN_PRINTF(__ERROR,"Can't read value from address: %p, size: %d\n", pointer, TypeSizeArg);
    }

    return static_cast<llvm_value_type>(value);
}

bool fastWrite2RemoteMem(const uintptr_t addr, const char* sink, const size_t size){
    assert_1message(sink != nullptr, "sink buffer is NULL. Check please.")
    assert_printf(client->fastWrite2RemoteMem(addr, sink, size),
                  "can't write array bytes [%d] to address: %p\n", size, addr);
    return true;
}

bool fastLoadFromRemoteMem(const uintptr_t addr, const size_t size, char* dist){
    assert_1message(dist != nullptr, "distination buffer is NULL. Check please.")
    assert_printf(client->fastLoadFromRemoteMem(addr, size, dist),
                  "can't read array bytes [%d] from address: %p\n", size, addr);
    return true;
}

} //namespace



extern "C" void __adin_store_(llvm_pass_addr pointer, llvm_value_type value, llvm_pass_arg TypeSizeArg, llvm_pass_arg AlignmentArg)
{
    adin::ADIN_PRINTF(adin::__DEBUG, "__store__ : pointer = %p, value 0x%X, TypeSizeArg %d, AlignmentArg %d\n", pointer, value, TypeSizeArg, AlignmentArg );
    adin::store(reinterpret_cast<llvm_ocd_addr>(pointer),
                   value, TypeSizeArg, AlignmentArg);
}



extern "C" llvm_value_type __adin_load_(const llvm_pass_addr pointer, llvm_pass_arg TypeSizeArg, llvm_pass_arg AlignmentArg)
{
    adin::ADIN_PRINTF(adin::__DEBUG, "__load__: pointer = %p, TypeSizeArg %d, AlignmentArg %d\n", pointer, TypeSizeArg, AlignmentArg);
    return adin::load(reinterpret_cast<llvm_ocd_addr>(pointer),
                  TypeSizeArg, AlignmentArg);
}
