#include <vector>


#include "RemoteAddress.h"
#include "logger.h"


using namespace std;

namespace remcu {

typedef struct {
    llvm_ocd_addr startAddr;
    llvm_ocd_addr endAddr;
} AddrInterval;

static vector<AddrInterval> adinIntervals;

//todo: safe from idiots!
bool add_to_adin_interval(const llvm_ocd_addr start, const llvm_ocd_addr end){
    if(start >= end){
        ADIN_PRINTF(__ERROR, "Error init virtual address : %p, %p\n", start, end);
        return false;
    }

    AddrInterval st = {start, end};
    adinIntervals.push_back(st);
    return true;
}

void clear_all_adin_interval(){
    adinIntervals.clear();
}

bool is_entry_adin_interval(const llvm_ocd_addr addr){
    for( const AddrInterval & i : adinIntervals ){
        bool isEntry = addr >= i.startAddr;
        isEntry &= addr < i.endAddr;
        if(isEntry)
            return true;
    }
    return false;
}

bool is_empty_adin_interval(){
    return adinIntervals.empty();
}


} //namespace
