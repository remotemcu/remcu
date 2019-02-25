#ifndef REMOTEADDRESS_H
#define REMOTEADDRESS_H

#include "AddressInterceptPass.h"


namespace remcu {

bool add_to_adin_interval(const llvm_ocd_addr start, const llvm_ocd_addr end);

void clear_all_adin_interval();

bool is_entry_adin_interval(const llvm_ocd_addr addr);

bool is_empty_adin_interval();

} //namespace

#endif // REMOTEADDRESS_H
