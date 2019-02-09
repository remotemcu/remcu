#ifndef OCDLIB_H
#define OCDLIB_H

#include <string>

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

enum LevelDebug { __ERROR = 0,
                  __WARNING, __INFO,
                  __DEBUG, __ALL_LOG };

#define _DEFAULT_TIMEOUT_SEC 3


bool connect2OpenOCD(const std::string host, const uint16_t port,
                     const int timeout_sec = _DEFAULT_TIMEOUT_SEC);

bool connect2GDB(const std::string host, const uint16_t port,
                 const int timeout_sec = _DEFAULT_TIMEOUT_SEC);

bool setMCU(const std::string target);
void clearMCU();

std::string getCurrentMCU();

bool resetRemoteUnit(const ResetType type);

void setVerboseLevel(const LevelDebug level);

bool arrayWrite2RemoteMem(const uintptr_t addr, const char* sink, const size_t size);

bool arrayLoadFromRemoteMem(const uintptr_t addr, const size_t size, char* dist);

} //namespace
#endif // OCDLIB_H
