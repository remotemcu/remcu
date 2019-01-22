#ifndef OCDLIB_H
#define OCDLIB_H

#include <string>

#include "AddressInterceptPass.h"

namespace adin {

enum ServerType {
    _DUMMY_SERVVER = 0,
    _OPENOCD_SERVER,
    _GDB_SERVER
};

bool connect2Server(std::string host, uint16_t port, ServerType server = _DUMMY_SERVVER, bool logo = false, int timeout = 0);

inline bool connect2OpenOCD(std::string host, uint16_t port, bool logo = true, int timeout = 0){
    return connect2Server(host, port, _OPENOCD_SERVER, logo, timeout);
}

void addInterceptAddress2Interval(llvm_ocd_addr start, llvm_ocd_addr end);

void clearAllInterceptAddressInterval();

enum LevelDebug { _ERROR = 0, _WARNING, _INFO, _DEBUG, _ALL_LOG };

typedef bool (*ErrorFunction_t)(const char *__assertion, const char *__file,
                                 unsigned int __line, const char *__function);

void setErrorFunction(ErrorFunction_t callback);

void setVerboseLevel(LevelDebug level);

bool fastWrite2RemoteMem(uintptr_t addr, const char* sink, size_t size);

bool fastLoadFromRemoteMem(uintptr_t addr, size_t size, char* dist);

} //namespace
#endif // OCDLIB_H
