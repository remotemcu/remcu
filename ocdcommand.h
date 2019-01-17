#ifndef OCDCOMMAND_H
#define OCDCOMMAND_H

#include <vector>

#include "llvm.h"

namespace ocd_lib {

bool store2RemoteAddr(llvm_pass_t addr, llvm_pass_t value, llvm_pass_t sizeVal);

bool loadFromRemoteAddr(llvm_pass_t addr, llvm_pass_t & value, llvm_pass_t sizeVal);

} //namespace

#endif // OCDCOMMAND_H
