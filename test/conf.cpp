
#include "TargetFun.h"
#include "AddressInterval.h"

namespace remcu {

void setConfig(){
    clearConfig();
    add_to_mem_interval(0x20000000, 0x20000000 + 200); //SRAM
    add_to_adin_interval(0x20000000, 0x20000000 + 200); //ADIN
}

std::string getCurrentConfig(){
    return std::string("TEST");
}

} //namespace
