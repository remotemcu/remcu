#include <vector>


#include "RemoteAddress.h"
#include "logger.h"


using namespace std;

namespace remcu {

typedef struct {
    llvm_ocd_addr startAddr;
    llvm_ocd_addr endAddr;
} AddrInterval;

static vector<AddrInterval> intervals;


//todo: safe from idiots!
bool addInterceptAddress2Interval(const llvm_ocd_addr start, const llvm_ocd_addr end){
    if(start >= end){
        ADIN_PRINTF(__ERROR, "Error init virtual address : %p, %p\n", start, end);
        return false;
    }

    AddrInterval st = {start, end};
    intervals.push_back(st);
    return true;
}

void clearAllInterceptAddressInterval(){
    intervals.clear();
}

bool isEntryHalfInterval(const llvm_ocd_addr addr){
    for( const AddrInterval & i : intervals ){
        bool isEntry = addr >= i.startAddr;
        isEntry &= addr < i.endAddr;
        if(isEntry)
            return true;
    }
    return false;
}

bool isEmptyAddressInterval(){
    return intervals.empty();
}


} //namespace
