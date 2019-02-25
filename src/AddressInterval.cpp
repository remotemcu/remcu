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
static vector<AddrInterval> memIntervals;


static bool add_to_interval(vector<AddrInterval> & intervals, const llvm_ocd_addr start, const llvm_ocd_addr end){
    if(start >= end){
        ADIN_PRINTF(__ERROR, "Error init virtual address : %p, %p\n", start, end);
        return false;
    }

    AddrInterval st = {start, end};
    intervals.push_back(st);
    return true;
}

bool is_entry_interval(vector<AddrInterval> intervals, const llvm_ocd_addr addr){
    for( const AddrInterval & i : intervals ){
        bool isEntry = addr >= i.startAddr;
        isEntry &= addr < i.endAddr;
        if(isEntry)
            return true;
    }
    return false;
}

//todo: safe from idiots!
bool add_to_adin_interval(const llvm_ocd_addr start, const llvm_ocd_addr end){
    return add_to_interval(adinIntervals, start, end);
}

bool is_entry_adin_interval(const llvm_ocd_addr addr){
    return is_entry_interval(adinIntervals, addr);
}

void clear_all_adin_interval(){
    adinIntervals.clear();
}

bool is_empty_adin_interval(){
    return adinIntervals.empty();
}

//todo: safe from idiots!
bool add_to_mem_interval(const llvm_ocd_addr start, const llvm_ocd_addr end){
    return add_to_interval(memIntervals, start, end);
}

bool is_entry_mem_interval(const llvm_ocd_addr addr){
    return is_entry_interval(memIntervals, addr);
}

void clear_all_mem_interval(){
    memIntervals.clear();
}

bool is_empty_mem_interval(){
    return memIntervals.empty();
}

} //namespace
