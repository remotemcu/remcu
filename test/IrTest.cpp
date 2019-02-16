
#include <stdint.h>
#include <assert.h>
#include <stdio.h>

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

int irTest(int* addr){

    const uint32_t  VALUE32 = 0x12345678;
    const uint8_t   VALUE8  = VALUE32 & 0xFF;
    const uint16_t  VALUE16 = VALUE32 & 0xFFFF;

    test_unpack_struct * unpack = reinterpret_cast<test_unpack_struct*>(addr);
    test_pack_struct * pack = reinterpret_cast<test_pack_struct*>(addr);

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