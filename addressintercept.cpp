
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>
#include <assert.h>
#include <vector>

#include "AddressInterceptPass.h"
#include "ocdcommand.h"
#include "logger.h"
#include "assertion.h"


using namespace std;

namespace adin {

typedef struct {
    llvm_ocd_addr startAddr;
    llvm_ocd_addr endAddr;
} AddrInterval;

static vector<AddrInterval> intervals;

void addInterceptAddress2Interval(llvm_ocd_addr start, llvm_ocd_addr end){
    AddrInterval st = {start, end};
    intervals.push_back(st);
}

void clearAllInterceptAddressInterval(){
    intervals.clear();
}

static bool isEntryHalfInterval(llvm_ocd_addr addr){
    for( const AddrInterval & i : intervals ){
        bool isEntry = addr >= i.startAddr;
        isEntry &= addr < i.endAddr;
        if(isEntry)
            return true;
    }
    return false;
}

static inline llvm_pass_arg loadLocalReturnValue(llvm_ocd_addr pointer, llvm_pass_arg TypeSizeArg, llvm_pass_arg AlignmentArg){

    llvm_pass_arg ret = 0;
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
    default:
        asser_1line(!"Size issue!");
        break;
    }

    return ret;
}


static inline void store(llvm_ocd_addr pointer, llvm_pass_arg value, llvm_pass_arg TypeSizeArg, llvm_pass_arg AlignmentArg)
{
    if(isEntryHalfInterval(pointer) == false){
        return;
    }

    asser_1line(store2RemoteAddr(pointer, value, TypeSizeArg));
}

static inline llvm_pass_arg load(llvm_ocd_addr pointer, llvm_pass_arg TypeSizeArg, llvm_pass_arg AlignmentArg)
{
    if(isEntryHalfInterval(pointer) == false)
        return loadLocalReturnValue(pointer, TypeSizeArg, AlignmentArg);

    llvm_pass_arg  value;
    asser_1line(loadFromRemoteAddr(pointer, value, TypeSizeArg));

    return value;
}


} //namespace



extern "C" void __adin_store_(llvm_pass_addr pointer, llvm_pass_arg value, llvm_pass_arg TypeSizeArg, llvm_pass_arg AlignmentArg)
{
    adin::ADIN_PRINTF(adin::_DEBUG, "__store__ : pointer = %p, value %d, TypeSizeArg %d, AlignmentArg %d\n", pointer, value, TypeSizeArg, AlignmentArg );
    adin::store(reinterpret_cast<llvm_ocd_addr>(pointer),
                   value, TypeSizeArg, AlignmentArg);
}



extern "C" llvm_pass_arg __adin_load_(const llvm_pass_addr pointer, llvm_pass_arg TypeSizeArg, llvm_pass_arg AlignmentArg)
{
    adin::ADIN_PRINTF(adin::_DEBUG, "__load__: pointer = %p, TypeSizeArg %d, AlignmentArg %d\n", pointer, TypeSizeArg, AlignmentArg);
    return adin::load(reinterpret_cast<llvm_ocd_addr>(pointer),
                  TypeSizeArg, AlignmentArg);
}
