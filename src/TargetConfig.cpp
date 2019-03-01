
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>
#include <assert.h>
#include <vector>

#include "AddressInterceptPass.h"
#include "logger.h"
#include "assertion.h"
#include "BaseClient.h"
#include "netwrapper.h"
#include "obusfaction.h"
#include "AddressInterval.h"

using namespace  std;

namespace remcu {

bool add_to_adin_interval(const llvm_ocd_addr start, const llvm_ocd_addr end);

void clear_all_adin_interval();

void clearConfig(){
    clear_all_adin_interval();
    clear_all_mem_interval();
}

static string targetMCU("unknown");

std::string getCurrentConfig(){
    return targetMCU;
}

bool setConfig(const std::string target){

    clearConfig();

    if(target.compare(_S_("TEST_CONFIG_MEM")) == 0){
        add_to_mem_interval(0x20000000, 0x20000000 + 200); //SRAM
        add_to_adin_interval(0x20000000, 0x20000000 + 200); //ADIN
        targetMCU.assign(target);
    } else  if(target.compare(_S_("STM32F40_41xxx")) == 0){
        add_to_adin_interval(0x20000000,  0x20000000 + (112)*1024); //SRAM
        add_to_adin_interval(0x40000000,  0x40008000); //APB1
        add_to_adin_interval(0x40010000,  0x40016C00); //APB2
        add_to_adin_interval(0x40020000,  0x40080000); //AHB1
        add_to_adin_interval(0x50000000,  0x50060C00); //AHB2
        add_to_adin_interval(0xA0000000,  0xA0001000); //AHB3
        targetMCU.assign(target);
    } else {
        return false;
    }
    return true;
}


}
