
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
