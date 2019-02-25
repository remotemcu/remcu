#ifndef REMOTEADDRESS_H
#define REMOTEADDRESS_H

#include "AddressInterceptPass.h"


namespace remcu {

bool addInterceptAddress2Interval(const llvm_ocd_addr start, const llvm_ocd_addr end);

void clearAllInterceptAddressInterval();

bool isEntryHalfInterval(const llvm_ocd_addr addr);

bool isEmptyAddressInterval();

} //namespace

#endif // REMOTEADDRESS_H
