#ifndef OCDLIB_H
#define OCDLIB_H

#include <string>

#include "AddressInterceptPass.h"

namespace adin {

bool connect2OpenOcd(std::string, uint16_t port, int timeout_sec = 0);

void addInterceptAddress2Interval(llvm_ocd_addr start, llvm_ocd_addr end);

void clearAllInterceptAddressInterval();

enum LevelDebug { _ERROR = 0, _WARNING, _INFO, _DEBUG, _ALL_LOG };

typedef bool (*ErrorFunction_t)(const char *__assertion, const char *__file,
                                 unsigned int __line, const char *__function);

void setErrorFunction(ErrorFunction_t callback);

void setVerboseLevel(LevelDebug level);

} //namespace
#endif // OCDLIB_H
