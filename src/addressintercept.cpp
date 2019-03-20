
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>
#include <vector>

#include "AddressInterceptPass.h"
#include "logger.h"
#include "assertion.h"
#include "BaseClient.h"
#include "ErrorFunction.h"
#include "AddressInterval.h"
#include "netwrapper.h"


using namespace std;

namespace remcu {

static ClientDummy dummy;
static ClientOpenOCD openocd;
static ClientGDB gdb;

static ClientBase * client = static_cast<ClientBase*>(&dummy);

bool connect2Server(const std::string host, const uint16_t port, const ServerType server,
                    const int timeout_sec){

    client->close();

    if(server == ServerType::_DUMMY_SERVVER){
        client = static_cast<ClientBase*>(&dummy);
    } else if (server == ServerType::_OPENOCD_SERVER) {
        client = static_cast<ClientBase*>(&openocd);
    } else if(server == ServerType::_GDB_SERVER) {
        client = static_cast<ClientBase*>(&gdb);
    } else {
        ADIN_LOG(__ERROR) << _S_("unknown client, please choice _OPENOCD_SERVER or _GDB_SERVER");
        return false;
    }

    return client->connect(host,port, timeout_sec);
}

bool disconnect(){
    const bool succes = closeTCP();
    ADIN_PRINTF(__INFO, "Close connection: %b\n", succes);
    client = static_cast<ClientBase*>(&dummy);
    return succes;
}

bool pingServer(){
    ADIN_PRINTF(__INFO, "Ping server ... \n", 0);
    return client->ping();
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
        ADIN_LOG(__ERROR) << _S_("Unknown size of type: ") << TypeSizeArg;
        ADIN_PRINTF(__ERROR, "at the uka: %p\n", pointer);
        break;
    }

    return ret;
}

static inline bool storeToLocalValue(const llvm_ocd_addr pointer, const llvm_value_type value,  const llvm_pass_arg TypeSizeArg, const llvm_pass_arg DECL_UNUSED AlignmentArg){

    switch (TypeSizeArg) {
    case 1:
        *reinterpret_cast<uint8_t*>(pointer) = value & 0x1;
        break;
    case 8:
        *reinterpret_cast<uint8_t*>(pointer) = value & 0xFF;
        break;
    case 16:
        *reinterpret_cast<uint16_t*>(pointer) = value & 0xFFFF;
        break;
    case 32:
        *reinterpret_cast<uint32_t*>(pointer) = value & 0xFFFFFFFF;
        break;
    case 64:
        *reinterpret_cast<uint64_t*>(pointer) = value;
        break;
    default:
        ADIN_LOG(__ERROR) << _S_("Unknown size of type: ") << TypeSizeArg;
        ADIN_PRINTF(__ERROR, "at the uka: %p\n", pointer);
        return false;
    }

    return true;
}

static llvm_value_type getMask(llvm_pass_arg TypeSizeArg){
    llvm_value_type ret = 0;
    switch (TypeSizeArg) {
    case 1:
        ret = 0x1;
        break;
    case 8:
        ret = 0xFF;
        break;
    case 16:
        ret = 0xFFFF;
        break;
    case 32:
        ret = 0xFFFFFFFF;
        break;
    case 64:
        ret = 0xFFFFFFFFFFFFFFFF;
        break;
    default:
        ADIN_LOG(__ERROR) << _S_("Unknown size of type: ") << TypeSizeArg;
        ADIN_LOG(__ERROR) << _S_("Mask isn't set!");
        ret = 0;
        break;
    }

    return ret;
}

static inline bool store(const llvm_ocd_addr pointer, const llvm_value_type value, const llvm_pass_arg TypeSizeArg, const llvm_pass_arg DECL_UNUSED AlignmentArg)
{
    assert_1message(is_empty_adin_interval() == false, "MCU is not set. Please input kind of MCU.");

    const llvm_pass_arg val = value & getMask(TypeSizeArg);

    if(is_entry_adin_interval(pointer) == false){
        ADIN_LOG(__INFO) << _S_("_s lo, u : ") <<  hex << pointer;
        assert_1message(storeToLocalValue(pointer, value, TypeSizeArg, AlignmentArg), "error local load");
        return true;
    }

    assert_printf(client->store2RemoteAddr(pointer, val, TypeSizeArg),
                  "Can't write value to u: %p, raz: %d\n", pointer, TypeSizeArg);
    return true;
}

static inline bool load(const llvm_ocd_addr pointer, llvm_value_type & value, const llvm_pass_arg TypeSizeArg, const  llvm_pass_arg AlignmentArg)
{
    assert_1message(is_empty_adin_interval() == false, "MCU is not set. Please input kind of MCU.");

    bool success = true;

    if(is_entry_adin_interval(pointer) == false){
        ADIN_LOG(__INFO) << _S_("_l lo, u : ") <<  hex << pointer;
        value = loadLocalReturnValue(pointer, TypeSizeArg, AlignmentArg);
    } else if(client->loadFromRemoteAddr(pointer, value, TypeSizeArg) == false){
        value = 0;
        ADIN_PRINTF(__ERROR,"Can't read value from u: %p, raz: %d\n", pointer, TypeSizeArg);
        success = false;
    }

    value &= getMask(TypeSizeArg);

    return success;
}

bool store2addr(const uintptr_t addr, const uint8_t* sink, const size_t size){
    assert_1message(sink != nullptr, "sink buffer is NULL. Check please.");

    assert_printf(client->arrayWrite2RemoteMem(addr, sink, size),
                  "can't write array bytes [%d] to address: %p\n", size, addr);
    return true;
}

bool loadFromAddr(const uintptr_t addr, const size_t size, uint8_t* dist){
    assert_1message(dist != nullptr, "distination buffer is NULL. Check please.");

    assert_printf(client->arrayLoadFromRemoteMem(addr, size, dist),
                  "can't read array bytes [%d] from address: %p\n", size, addr);
    return true;
}

static const llvm_pass_arg SIZE_MEM_FUN_OPERAND = 8;
static const llvm_pass_arg ALIGN_MEM_FUN_OPERAND = 1;

bool adin_memcpy(llvm_pass_addr dest, const llvm_pass_addr src, const llvm_pass_arg size){
    llvm_value_type tmp = 0;
    for(llvm_pass_arg i=0; i < size; i++){
        assert_1message(
            load(reinterpret_cast<llvm_ocd_addr>(src+i), tmp, SIZE_MEM_FUN_OPERAND, ALIGN_MEM_FUN_OPERAND),
            "error laod to mcp");
        tmp &= 0xFF;
        assert_1message(
            store(reinterpret_cast<llvm_ocd_addr>(dest+i), tmp, SIZE_MEM_FUN_OPERAND, ALIGN_MEM_FUN_OPERAND),
            "error write to mcp");
    }
    return true;
}

bool adin_memset(llvm_pass_addr dest, const llvm_pass_arg val, const llvm_pass_arg size){
    const uint8_t val8 = val & 0xFF;
    for(llvm_pass_arg i=0; i < size; i++){
        assert_1message(
            store(reinterpret_cast<llvm_ocd_addr>(dest+i), val8, SIZE_MEM_FUN_OPERAND, ALIGN_MEM_FUN_OPERAND),
            "error mse");
    }
    return true;
}

} //namespace

using namespace remcu;

extern "C" void __adin_store_(llvm_pass_addr pointer, llvm_value_type value, llvm_pass_arg TypeSizeArg, llvm_pass_arg AlignmentArg)
{
    ADIN_PRINTF(__INFO, ">>> : u %p, z 0x%X, r %d, a %d\n", pointer, value, TypeSizeArg, AlignmentArg );
    const bool success = store(reinterpret_cast<llvm_ocd_addr>(pointer),
                   value, TypeSizeArg, AlignmentArg);

    if(success == false){
        errorAppear();
    }
}



extern "C" llvm_value_type __adin_load_(const llvm_pass_addr pointer, llvm_pass_arg TypeSizeArg, llvm_pass_arg AlignmentArg)
{
    ADIN_PRINTF(__INFO, "<<< : u %p, r %d, a %d\n", pointer, TypeSizeArg, AlignmentArg);
    llvm_value_type value = 0;
    const bool success = load(reinterpret_cast<llvm_ocd_addr>(pointer), value,
                  TypeSizeArg, AlignmentArg);

    if(success == false){
        value = 0;
        errorAppear();
    }

    ADIN_PRINTF(__INFO, "_L : av 0x%X\n", value);

    return value;
}

extern "C" void __adin_memcpy_(llvm_pass_addr dest, const llvm_pass_addr src, const llvm_pass_arg size)
{
    ADIN_PRINTF(__INFO, "mc : ud %p, us %p, r %d\n", dest, src, size);
    const bool success = adin_memcpy(dest, src, size);
    if(success == false){
        errorAppear();
    }
}
#if 0
extern "C" void __adin_memmove_(llvm_pass_addr dest, const llvm_pass_addr src, const llvm_pass_arg size)
{
    ADIN_PRINTF(__INFO, "mm : ud %p, us %p, r %d\n", dest, src, size);

}
#endif
extern "C" void __adin_memset_(llvm_pass_addr dest, const llvm_pass_arg val, const llvm_pass_arg size)
{
    ADIN_PRINTF(__INFO, "ms : u %p, v %x, r %d\n", dest, val, size);
    const bool success = adin_memset(dest, val, size);
    if(success == false){
        errorAppear();
    }
}
