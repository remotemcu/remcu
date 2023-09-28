
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "remcu_exports_symbol_enter.h"
#include "Ir.h"
#include "remcu_exports_symbol_exit.h"


void simpleTest(int* addr){
    *addr = 0;
    *addr = *(addr+1);
}


static void testU8(uint8_t* addr){
    *addr = 0xFF;
    assert(*addr == 0xFF);
    *addr = 0;
    assert(*addr == 0);
}

static void testU16(uint16_t* addr){
    *addr = 0xFFFF;
    assert(*addr == 0xFFFF);
    *addr = 0;
    assert(*addr == 0);
}

static void testU32(uint32_t* addr){
    *addr = 0xFFFFFFFF;
    assert(*addr == 0xFFFFFFFF);
    *addr = 0;
    assert(*addr == 0);
}

void testStoreLoadAllType(uintptr_t addr){
    testU8(reinterpret_cast<uint8_t*>(addr));
    testU16(reinterpret_cast<uint16_t*>(addr));
    testU32(reinterpret_cast<uint32_t*>(addr));
}