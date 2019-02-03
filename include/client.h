#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <stdint.h>
#include <vector>

#include "AddressInterceptPass.h"
#include "adin.h"

namespace adin {

struct ClientBase {

    bool connect(const std::string, const uint16_t port, int timeout_sec) const ;

    virtual bool store2RemoteAddr(const llvm_ocd_addr addr, const llvm_pass_arg value, const llvm_pass_arg sizeVal) const ;

    virtual bool loadFromRemoteAddr(const llvm_ocd_addr addr, llvm_pass_arg & value, const llvm_pass_arg sizeVal) const ;

    virtual bool fastWrite2RemoteMem(const uintptr_t addr, const char* sink, const size_t size) const ;

    virtual bool fastLoadFromRemoteMem(const uintptr_t addr, const size_t size, char* dist) const ;

    virtual bool resetRemoteUnit(const ResetType) const ;

    bool close() const ;

protected:
    static std::vector<char> bufferSend;
    static std::vector<char> bufferReceiv;
};

struct ClientDummy: ClientBase {

    bool connect(const std::string __attribute__((unused)), const uint16_t __attribute__((unused)), int __attribute__((unused))) const {return true;}

    bool store2RemoteAddr(const llvm_ocd_addr __attribute__((unused)), const llvm_pass_arg __attribute__((unused)), const llvm_pass_arg __attribute__((unused))) const {return true;}

    bool loadFromRemoteAddr(const llvm_ocd_addr __attribute__((unused)), llvm_pass_arg & value, const llvm_pass_arg __attribute__((unused))) const {value = 0; return true;}

    bool fastWrite2RemoteMem(const uintptr_t __attribute__((unused)), const char* __attribute__((unused)), const size_t __attribute__((unused))) const {return true;}

    bool fastLoadFromRemoteMem(const uintptr_t __attribute__((unused)), const size_t __attribute__((unused)), char* __attribute__((unused))) const {return true;}

    bool resetRemoteUnit(const ResetType __attribute__((unused))) const {return true;}

    bool close() const {return true;}
};

struct ClientOpenOCD: ClientBase {

    //bool connect(const std::string, const uint16_t port, int timeout_sec) const ;

    bool store2RemoteAddr(const llvm_ocd_addr addr, const llvm_pass_arg value, const llvm_pass_arg sizeVal) const ;

    bool loadFromRemoteAddr(const llvm_ocd_addr addr, llvm_pass_arg & value, const llvm_pass_arg sizeVal) const ;

    bool fastWrite2RemoteMem(const uintptr_t addr, const char* sink, const size_t size) const ;

    bool fastLoadFromRemoteMem(const uintptr_t addr, const size_t size, char* dist) const ;

    bool resetRemoteUnit(const ResetType) const ;

    //bool close() const ;
};

struct ClientGDB: ClientBase {

    //bool connect(const std::string, const uint16_t port, int timeout_sec) const ;

    bool store2RemoteAddr(const llvm_ocd_addr addr, const llvm_pass_arg value, const llvm_pass_arg sizeVal) const ;

    bool loadFromRemoteAddr(const llvm_ocd_addr addr, llvm_pass_arg & value, const llvm_pass_arg sizeVal) const ;

    bool fastWrite2RemoteMem(const uintptr_t addr, const char* sink, const size_t size) const ;

    bool fastLoadFromRemoteMem(const uintptr_t addr, const size_t size, char* dist) const ;

    bool resetRemoteUnit(const ResetType) const ;

    //bool close() const ;
};

} //namespace

#endif // CLIENT_H
