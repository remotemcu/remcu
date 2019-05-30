
#include <stdlib.h>
#include <string>
#include <cstring>
#include <vector>

#include "logger.h"
#include "assertion.h"
#include "AddressInterval.h"
#include "AddressFunction.h"


namespace  remcu {


bool store2mem(const uintptr_t addr, const uint8_t* sink, const size_t size){
    assert_1message(size < _MEM_OPERATION_SIZE, "Try write a lot of bytes to remote memory. Please decrease size of array.");

    assert_1message(is_entry_mem_interval(addr), "Try write to non-memory region");
    assert_1message(is_entry_mem_interval(addr + size), "Try write to non-memory region.");

    return store2addr(addr, sink, size);
}


bool loadFrMem(const uintptr_t addr, const size_t size, uint8_t* dist){
    assert_1message(size < _MEM_OPERATION_SIZE, "Try read a lot of bytes from remote memory. Please decrease size of array.");

    assert_1message(is_entry_mem_interval(addr), "Try read from non-memory region");
    assert_1message(is_entry_mem_interval(addr + size), "Try read from non-memory region.");

    return loadFromAddr(addr, size, dist);
}


} //namespace
