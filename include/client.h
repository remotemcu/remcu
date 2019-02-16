#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <stdint.h>
#include <vector>

#include "AddressInterceptPass.h"
#include "exports.h"
#include "defines.h"

namespace remcu {

struct ClientBase {

    bool connect(const std::string, const uint16_t port, int timeout_sec) const ;

    virtual bool store2RemoteAddr(const llvm_ocd_addr addr, const llvm_value_type value, const llvm_pass_arg sizeVal) const ;

    virtual bool loadFromRemoteAddr(const llvm_ocd_addr addr, llvm_value_type & value, const llvm_pass_arg sizeVal) const ;

    virtual bool arrayWrite2RemoteMem(const uintptr_t addr, const char* sink, const size_t size) const ;

    virtual bool arrayLoadFromRemoteMem(const uintptr_t addr, const size_t size, char* dist) const ;

    virtual bool resetRemoteUnit(const ResetType) const ;

    bool close() const ;

protected:
    static std::vector<char> bufferSend;
    static std::vector<char> bufferReceiv;
};

struct ClientDummy: ClientBase {

    bool connect(const std::string DECL_UNUSED, const uint16_t DECL_UNUSED, int DECL_UNUSED) const {return true;}

    bool store2RemoteAddr(const llvm_ocd_addr DECL_UNUSED, const llvm_value_type DECL_UNUSED, const llvm_pass_arg DECL_UNUSED) const {return true;}

    bool loadFromRemoteAddr(const llvm_ocd_addr DECL_UNUSED, llvm_value_type & value, const llvm_pass_arg DECL_UNUSED) const {value = 0; return true;}

    bool arrayWrite2RemoteMem(const uintptr_t DECL_UNUSED, const char* DECL_UNUSED, const size_t DECL_UNUSED) const {return true;}

    bool arrayLoadFromRemoteMem(const uintptr_t DECL_UNUSED, const size_t DECL_UNUSED, char* DECL_UNUSED) const {return true;}

    bool resetRemoteUnit(const ResetType DECL_UNUSED) const {return true;}

    bool close() const {return true;}
};

struct ClientOpenOCD: ClientBase {

    bool store2RemoteAddr(const llvm_ocd_addr addr, const llvm_value_type value, const llvm_pass_arg sizeVal) const ;

    bool loadFromRemoteAddr(const llvm_ocd_addr addr, llvm_value_type & value, const llvm_pass_arg sizeVal) const ;

    bool arrayWrite2RemoteMem(const uintptr_t addr, const char* sink, const size_t size) const ;

    bool arrayLoadFromRemoteMem(const uintptr_t addr, const size_t size, char* dist) const ;

    bool resetRemoteUnit(const ResetType) const ;

};

struct ClientGDB: ClientBase {

    bool store2RemoteAddr(const llvm_ocd_addr addr, const llvm_value_type value, const llvm_pass_arg sizeVal) const ;

    bool loadFromRemoteAddr(const llvm_ocd_addr addr, llvm_value_type & value, const llvm_pass_arg sizeVal) const ;

    bool arrayWrite2RemoteMem(const uintptr_t addr, const char* sink, const size_t size) const ;

    bool arrayLoadFromRemoteMem(const uintptr_t addr, const size_t size, char* dist) const ;

    bool resetRemoteUnit(const ResetType) const ;

};

bool commandSendAndGetResponse(const char * data, const size_t lenData,
                               std::vector<char> & bufferResp, size_t & lenResp, const char token);


} //namespace

#endif // CLIENT_H
