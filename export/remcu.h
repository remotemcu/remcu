#ifndef REMCU_H
#define REMCU_H

#include <stdint.h>
#include <stdlib.h>

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

#ifdef __cplusplus
extern "C" {
#endif

enum ResetType {
    __RUN = 0,
    __HALT,
};

enum LevelDebug { __ERROR = 0,
                  __WARNING, __INFO,
                  __DEBUG, __ALL_LOG };

typedef void (*ErrorSignalFunc)();

#define _SIZE_ONE_MEMPCY 33

#define _DEFAULT_TIMEOUT_SEC 3

REMCULIB_DLL_API bool remcu_connect2OpenOCD(const char* host, const uint16_t port,
                      const int timeout_sec);

REMCULIB_DLL_API bool remcu_connect2GDB(const char* host, const uint16_t port,
                       const int timeout_sec);

REMCULIB_DLL_API bool remcu_disconnect();

REMCULIB_DLL_API bool remcu_is_connected();

REMCULIB_DLL_API const char * remcu_getVersion();

REMCULIB_DLL_API bool remcu_resetRemoteUnit(const enum ResetType type);

REMCULIB_DLL_API void remcu_setVerboseLevel(const enum LevelDebug level);

REMCULIB_DLL_API void remcu_setErrorSignalFunc(ErrorSignalFunc callback);

REMCULIB_DLL_API size_t remcu_getErrorCout();

REMCULIB_DLL_API void remcu_clearErrorCount();

REMCULIB_DLL_API bool remcu_store2mem(const uintptr_t addr, const uint8_t* sink, const size_t size);

REMCULIB_DLL_API bool remcu_loadFrMem(const uintptr_t addr, const size_t size, uint8_t* dist);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // REMCU_H
