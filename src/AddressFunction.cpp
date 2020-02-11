
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
    assert_1message(size <= MAX_MEM_OPERATION_SIZE, "The record size limit exceeded at a time. It is more then MAX_MEM_OPERATION_SIZE. Copy a lot of data in parts.");
    assert_1message(size >= MIN_MEM_OPERATION_SIZE, "Invalid record size. It is less then MIN_MEM_OPERATION_SIZE. Increase size of array.");

    assert_1message(is_entry_mem_interval(addr), "Invalid memory address. Trying to write to non-memory(RAM & ROM) region");
    assert_1message(is_entry_mem_interval(addr + size), "Invalid memory address. Trying to write to non-memory(RAM & ROM) region!");

    return store2addr(addr, sink, size);
}


bool loadFrMem(const uintptr_t addr, const size_t size, uint8_t* dist){
    assert_1message(size <= MAX_MEM_OPERATION_SIZE, "The read size limit exceeded at a time. It is more then MAX_MEM_OPERATION_SIZE. Copy a lot of data in parts.");
    assert_1message(size >= MIN_MEM_OPERATION_SIZE, "Invalid read size. It is less then MIN_MEM_OPERATION_SIZE. Increase size of array.");

    assert_1message(is_entry_mem_interval(addr), "Invalid memory address. Trying to read from non-memory(RAM & ROM) region");
    assert_1message(is_entry_mem_interval(addr + size), "Invalid memory address. Trying to read from non-memory(RAM & ROM) region!");

    return loadFromAddr(addr, size, dist);
}


} //namespace
