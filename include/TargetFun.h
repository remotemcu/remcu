#ifndef TARGETFUN_H
#define TARGETFUN_H

#include <string>

namespace remcu {

void setConfig();

uint32_t get_RAM_addr_for_test();

std::string getCurrentConfig();

} //namespace
#endif // TARGETFUN_H
