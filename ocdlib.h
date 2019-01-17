#ifndef OCDLIB_H
#define OCDLIB_H
#include "AddressInterceptPass.h"

namespace ocd_lib {

void addInterceptAddress2Interval(llvm_ocd_addr start, llvm_ocd_addr end);

void clearAllInterceptAddressInterval();

} //namespace
#endif // OCDLIB_H
