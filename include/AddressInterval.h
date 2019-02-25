#ifndef REMOTEADDRESS_H
#define REMOTEADDRESS_H

#include "AddressInterceptPass.h"


namespace remcu {

bool add_to_adin_interval(const llvm_ocd_addr start, const llvm_ocd_addr end);

void clear_all_adin_interval();

bool is_entry_adin_interval(const llvm_ocd_addr addr);

bool is_empty_adin_interval();



bool add_to_mem_interval(const llvm_ocd_addr start, const llvm_ocd_addr end);

bool is_entry_mem_interval(const llvm_ocd_addr addr);

void clear_all_mem_interval();

bool is_empty_mem_interval();

#define _SIZE_ONE_MEMPCY 33

} //namespace

#endif // REMOTEADDRESS_H
