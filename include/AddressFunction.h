#ifndef ADDRESSFUNCTION_H
#define ADDRESSFUNCTION_H

#include <stdlib.h>

#include "AddressInterceptPass.h"
#include "remcu.h"

namespace remcu {

bool store2addr(const uintptr_t addr, const uint8_t* sink, const size_t size);

bool loadFromAddr(const uintptr_t addr, const size_t size, uint8_t* dist);

} //namespace
#endif // ADDRESSFUNCTION_H
