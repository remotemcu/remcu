
#include <stdint.h>
#include <assert.h>
#include <stdio.h>

#include "IrTest.h"

typedef struct {
    int8_t  field_1_8;
    int16_t field_2_16;
    int32_t field_3_32;
} test_unpack_struct;

typedef struct __attribute__((packed)) {
    int8_t  field_1_8;
    int16_t field_2_16;
    int32_t field_3_32;
} test_pack_struct;

#define A1_VALUE 0x12345678
#define A2_VALUE 0x87654321

struct {
    unsigned int        a1;
    unsigned int        a2;
} test_struct_load = {.a1 = A1_VALUE, .a2 = A2_VALUE};

#define LONG_VALUE 0x123456789ABCDEF

uint64_t test_global_var = LONG_VALUE;


int irTest(int* addr){

    const uint32_t  VALUE32 = 0x12345678;
    const uint8_t   VALUE8  = VALUE32 & 0xFF;
    const uint16_t  VALUE16 = VALUE32 & 0xFFFF;

    test_unpack_struct * unpack = reinterpret_cast<test_unpack_struct*>(addr);
    test_pack_struct * pack = reinterpret_cast<test_pack_struct*>(addr);

    printf("\ntest_global_var: %p - v: %lx \n\n",&test_global_var, test_global_var);

    assert(test_global_var == LONG_VALUE);

    printf("\ntest_struct_load: %p - a1: %x - a2: %x\n\n",&test_struct_load, test_struct_load.a1, test_struct_load.a2 );

    assert(test_struct_load.a1 == A1_VALUE);

    *addr = 0;
    *addr = VALUE32;
    assert(*addr == VALUE32);

    *addr = 0;
    *reinterpret_cast<uint8_t*>(addr) = VALUE8;
    assert(*addr == VALUE8);

    *addr = 0;
    *reinterpret_cast<uint16_t*>(addr) = VALUE16;
    assert(*addr == VALUE16);

    assert(sizeof (test_unpack_struct) != sizeof (test_pack_struct));
    assert(sizeof (test_unpack_struct)  == 8);
    assert(sizeof (test_pack_struct)    == 7);

    addr[0] = 0;
    addr[1] = 0;
    unpack->field_1_8   = VALUE8;
    unpack->field_2_16  = VALUE16;
    unpack->field_3_32  = VALUE32;

    assert(addr[0] == (
                          (VALUE16 << 16) | VALUE8)
           );
    assert(addr[1] == VALUE32);

    addr[0] = 0;
    addr[1] = 0;
    pack->field_1_8   = VALUE8;
    pack->field_2_16  = VALUE16;
    pack->field_3_32  = VALUE32;

    assert(addr[0] == ( (VALUE32 << 24) | (VALUE16 << 8) | VALUE8) );
    assert(addr[1] == (VALUE32 >> 8));

    *addr = 0;
    return *addr;
}

void irTestSimple(int* addr){
    *addr = 0;
    *addr = *(addr+1);
}
