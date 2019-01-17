#ifndef OCDCOMMAND_H
#define OCDCOMMAND_H

#include <vector>

#include "AddressInterceptPass.h"

namespace ocd_lib {

bool store2RemoteAddr(llvm_ocd_addr addr, llvm_pass_arg value, llvm_pass_arg sizeVal);

bool loadFromRemoteAddr(llvm_ocd_addr addr, llvm_pass_arg & value, llvm_pass_arg sizeVal);

} //namespace

#endif // OCDCOMMAND_H
