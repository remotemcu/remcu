
#include "target.h"
#include "AddressInterval.h"

namespace remcu {

void setConfig(){
    add_to_mem_interval(0x20000000, 0x20000000 + 200); //SRAM
    add_to_adin_interval(0x20000000, 0x20000000 + 200); //ADIN
}

uint32_t get_RAM_addr_for_test(){
    return 0x20000000;
}

} //namespace
