#ifndef OCDLIB_H
#define OCDLIB_H

#include <string>

namespace adin {

#if (defined(_WIN32) || defined(WIN32) || defined(WIN64) || defined(_WIN64))
#define _DLL_EXPORT __declspec(dllexport)
#else
#define _DLL_EXPORT
#endif

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


bool _DLL_EXPORT connect2OpenOCD(const std::string host, const uint16_t port,
                     const int timeout_sec = _DEFAULT_TIMEOUT_SEC);

bool _DLL_EXPORT connect2GDB(const std::string host, const uint16_t port,
                 const int timeout_sec = _DEFAULT_TIMEOUT_SEC);

bool _DLL_EXPORT setMCU(const std::string target);
void _DLL_EXPORT clearMCU();

std::string _DLL_EXPORT getCurrentMCU();

bool _DLL_EXPORT resetRemoteUnit(const ResetType type);

void _DLL_EXPORT setVerboseLevel(const LevelDebug level);

bool _DLL_EXPORT arrayWrite2RemoteMem(const uintptr_t addr, const char* sink, const size_t size);

bool _DLL_EXPORT arrayLoadFromRemoteMem(const uintptr_t addr, const size_t size, char* dist);

} //namespace
#endif // OCDLIB_H
