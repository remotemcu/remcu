#ifndef REMCU_H
#define REMCU_H

#include <string>

#ifndef __EXPORT
#define __EXPORT
#endif

#ifndef REMCULIB_DLL_API
  #if defined(_WIN32) || defined(WIN32)
    #define REMCULIB_DLL_API __declspec(dllimport)
  #else 
    #define REMCULIB_DLL_API
  #endif
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

typedef void (*ErrorSignalFunc)();


REMCULIB_DLL_API bool connect2OpenOCD(const std::string host, const uint16_t port,
                     const int timeout_sec = _DEFAULT_TIMEOUT_SEC);

REMCULIB_DLL_API bool connect2GDB(const std::string host, const uint16_t port,
                 const int timeout_sec = _DEFAULT_TIMEOUT_SEC);

REMCULIB_DLL_API bool disconnect();

REMCULIB_DLL_API std::string getVersion();

REMCULIB_DLL_API bool setConfig(const std::string target);

REMCULIB_DLL_API void clearConfig();

REMCULIB_DLL_API std::string getCurrentConfig();

REMCULIB_DLL_API bool resetRemoteUnit(const ResetType type);

REMCULIB_DLL_API void setVerboseLevel(const LevelDebug level);

REMCULIB_DLL_API void setErrorSignalFunc(ErrorSignalFunc callback);

REMCULIB_DLL_API size_t getErrorCout();

REMCULIB_DLL_API void clearErrorCount();

REMCULIB_DLL_API bool arrayWrite2RemoteMem(const uintptr_t addr, const char* sink, const size_t size);

REMCULIB_DLL_API bool arrayLoadFromRemoteMem(const uintptr_t addr, const size_t size, char* dist);

} //namespace
#endif // REMCU_H
