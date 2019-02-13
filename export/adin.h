#ifndef ADIN_LIB_H
#define ADIN_LIB_H

#include <string>

#ifndef __EXPORT
#define __EXPORT
#endif


namespace remcu {

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

enum LevelDebug { __ERROR = 0,
                  __WARNING, __INFO,
                  __DEBUG, __ALL_LOG };

#define _DEFAULT_TIMEOUT_SEC 3


bool __EXPORT connect2OpenOCD(const std::string host, const uint16_t port,
                     const int timeout_sec = _DEFAULT_TIMEOUT_SEC);

bool __EXPORT connect2GDB(const std::string host, const uint16_t port,
                 const int timeout_sec = _DEFAULT_TIMEOUT_SEC);

bool __EXPORT disconnect();

std::string __EXPORT getVersion();
//setConfig
bool __EXPORT setMCU(const std::string target);
void __EXPORT clearMCU();

std::string __EXPORT getCurrentMCU();

bool __EXPORT resetRemoteUnit(const ResetType type);

void __EXPORT setVerboseLevel(const LevelDebug level);

bool __EXPORT arrayWrite2RemoteMem(const uintptr_t addr, const char* sink, const size_t size);

bool __EXPORT arrayLoadFromRemoteMem(const uintptr_t addr, const size_t size, char* dist);

} //namespace
#endif // ADIN_LIB_H
