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

enum ResetType {
    __RUN = 0,
    __HALT,
    __INIT
};

enum LevelDebug { _ERROR = 0,
                  _WARNING, _INFO,
                  _DEBUG, _ALL_LOG };

#define _DEFAULT_TIMEOUT_SEC 3

bool connect2Server(const std::string host, const uint16_t port,
                    const ServerType server = _DUMMY_SERVVER, const bool logo = false,
                    const int timeout_sec = _DEFAULT_TIMEOUT_SEC);

inline bool connect2OpenOCD(const std::string host, const uint16_t port,
                            const bool logo = true, const int timeout_sec = _DEFAULT_TIMEOUT_SEC){
    return connect2Server(host, port, _OPENOCD_SERVER, logo, timeout_sec);
}

bool resetRemoteUnit(const ResetType type);

void addInterceptAddress2Interval(const llvm_ocd_addr start, const llvm_ocd_addr end);

void clearAllInterceptAddressInterval();

typedef bool (*ErrorFunction_t)(const char *__assertion, const char *__file,
                                const unsigned int __line, const char *__function);

void setVerboseLevel(const LevelDebug level);

bool fastWrite2RemoteMem(const uintptr_t addr, const char* sink, const size_t size);

bool fastLoadFromRemoteMem(const uintptr_t addr, const size_t size, char* dist);

} //namespace
#endif // OCDLIB_H
