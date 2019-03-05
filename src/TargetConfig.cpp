
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

#if 0
static string targetMCU("unknown");

std::string getCurrentConfig(){
    return targetMCU;
}


bool setConfig(const std::string target){

    clearConfig();
    bool set = false;
#ifdef _MCU_TYPE_TEST
    if(target.compare(_S_("TEST")) == 0){

        set = true;
    }
#elif defined(_MCU_TYPE_STM32F4_Discovery)
    if(target.compare(_S_("STM32F4_Discovery")) == 0){
        add_to_adin_interval(0x20000000,  0x20000000 + (112)*1024); //SRAM
        add_to_adin_interval(0x40000000,  0x40008000); //APB1
        add_to_adin_interval(0x40010000,  0x40016C00); //APB2
        add_to_adin_interval(0x40020000,  0x40080000); //AHB1
        add_to_adin_interval(0x50000000,  0x50060C00); //AHB2
        add_to_adin_interval(0xA0000000,  0xA0001000); //AHB3
        set = true;
    }
ELIFMCU(STM8L15X_MD)
    if(CHECK("STM8L15X_MD")) == 0){
        add_to_mem_interval(0, 0x800); //SRAM //SRAM
        add_to_adin_interval(0x800,  0x10000); //Perephi
        set = true;
    }
#else
    #error not set mcu lib!
#endif

    if(set == false) {
        ADIN_PRINTF(__ERROR, "unknown config", 1);
        return false;
    }

    targetMCU.assign(target);
    return true;
}
#endif

}
