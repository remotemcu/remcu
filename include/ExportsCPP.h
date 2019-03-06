#ifndef EXPORTS_H
#define EXPORTS_H

#include <string>

#include "defines.h"

#include "remcu.h"

namespace remcu {

 bool connect2OpenOCD(const std::string host, const uint16_t port,
                                      const int timeout_sec = _DEFAULT_TIMEOUT_SEC);

 bool connect2GDB(const std::string host, const uint16_t port,
                                  const int timeout_sec = _DEFAULT_TIMEOUT_SEC);

 bool disconnect();

 bool pingServer();

 std::string getVersion();

 bool setConfig(const std::string target);

 void clearConfig();

 bool resetRemoteUnit(const ResetType type);

 void setVerboseLevel(const LevelDebug level);

 void setErrorSignalFunc(ErrorSignalFunc callback);

 size_t getErrorCout();

 void clearErrorCount();

 bool store2mem(const uintptr_t addr, const uint8_t* sink, const size_t size);

 bool loadFrMem(const uintptr_t addr, const size_t size, uint8_t* dist);

} //namespace

#endif // EXPORTS_H
