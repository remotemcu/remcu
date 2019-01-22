#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <stdint.h>

#include "AddressInterceptPass.h"


namespace adin {

struct ClientBase {

    virtual bool connect(std::string, uint16_t port, int timeout_sec);

    virtual bool store2RemoteAddr(llvm_ocd_addr addr, llvm_pass_arg value, llvm_pass_arg sizeVal);

    virtual bool loadFromRemoteAddr(llvm_ocd_addr addr, llvm_pass_arg & value, llvm_pass_arg sizeVal);

    virtual bool fastWrite2RemoteMem(uintptr_t addr, const char* sink, size_t size);

    virtual bool fastLoadFromRemoteMem(uintptr_t addr, size_t size, char* dist);

    virtual bool close();
};

struct ClientDummy: ClientBase {

    bool connect(std::string __attribute__((unused)), uint16_t __attribute__((unused)), int __attribute__((unused))){return true;}

    bool store2RemoteAddr(llvm_ocd_addr __attribute__((unused)), llvm_pass_arg __attribute__((unused)), llvm_pass_arg __attribute__((unused))){return true;}

    bool loadFromRemoteAddr(llvm_ocd_addr __attribute__((unused)), llvm_pass_arg & value, llvm_pass_arg __attribute__((unused))){value = 0; return true;}

    bool fastWrite2RemoteMem(uintptr_t __attribute__((unused)), const char* __attribute__((unused)), size_t __attribute__((unused))){return true;}

    bool fastLoadFromRemoteMem(uintptr_t __attribute__((unused)), size_t __attribute__((unused)), char* __attribute__((unused))){return true;}

    bool close(){return true;}
};

struct ClientOpenOCD: ClientBase {

    bool connect(std::string, uint16_t port, int timeout_sec);

    bool store2RemoteAddr(llvm_ocd_addr addr, llvm_pass_arg value, llvm_pass_arg sizeVal);

    bool loadFromRemoteAddr(llvm_ocd_addr addr, llvm_pass_arg & value, llvm_pass_arg sizeVal);

    bool fastWrite2RemoteMem(uintptr_t addr, const char* sink, size_t size);

    bool fastLoadFromRemoteMem(uintptr_t addr, size_t size, char* dist);

    bool close();
};


} //namespace

#endif // CLIENT_H
