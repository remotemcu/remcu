#ifndef LLVM_H
#define LLVM_H
#include "stdint.h"


typedef  uint32_t llvm_pass_arg;
typedef  uint8_t * llvm_pass_addr;

typedef uintptr_t llvm_ocd_addr;

extern "C" void __adin_store_(llvm_pass_addr pointer, llvm_pass_arg value, llvm_pass_arg TypeSizeArg, llvm_pass_arg AlignmentArg);

extern "C" llvm_pass_arg __adin_load_(llvm_pass_addr pointer, llvm_pass_arg TypeSizeArg, llvm_pass_arg AlignmentArg);

#endif // LLVM_H
