
#include "logger.h"
#include "assertion.h"
#include "IrTest.h"

using namespace std;
using namespace remcu;
#if 0
static bool returnStr(char* str, size_t & len, const string & ret){
    const size_t fact_size = ret.size();
    const size_t need_size = fact_size + 1;

    if(len < need_size){
        ADIN_PRINTF(__ERROR, "buffer lenth is small. need : %d\n", need_size);
        len = 0;
        return false;
    }

    ret.copy(str, fact_size);
    str[need_size - 1] = '\0';
    len = need_size;
    return true;
}
#endif
extern "C"{

 bool remcu_connect2OpenOCD(const char* host, const uint16_t port,
                     const int timeout_sec){
     return remcu::connect2OpenOCD(std::string(host), port, timeout_sec);
 }

 bool remcu_connect2GDB(const char* host, const uint16_t port,
                  const int timeout_sec){
     return remcu::connect2GDB(std::string(host), port, timeout_sec);
 }

 bool remcu_disconnect(){
     return remcu::disconnect();
 }

 bool remcu_is_connected(){
     return remcu::pingServer();
 }

 const char * remcu_getVersion(){
     const static string version = remcu::getVersion();
     return version.c_str();
 }

 bool remcu_resetRemoteUnit(const ResetType type){
     return remcu::resetRemoteUnit(type);
 }

 void remcu_setVerboseLevel(const LevelDebug level){
     return remcu::setVerboseLevel(level);
 }

 void remcu_setErrorSignalFunc(ErrorSignalFunc callback){
     remcu::setErrorSignalFunc(callback);
 }

 size_t remcu_getErrorCount(){
     return remcu::getErrorCount();
 }

 void remcu_clearErrorCount(){
     remcu::clearErrorCount();
 }

 bool remcu_store2mem(const uintptr_t addr, const uint8_t* sink, const size_t size){
     return remcu::store2mem(addr, sink, size);
 }

 bool remcu_loadFrMem(const uintptr_t addr, const size_t size, uint8_t* dist){
     return remcu::loadFrMem(addr, size, dist);
 }

 const char* remcu_targetRAMtest(uintptr_t ramAddr){
     const char* ret = IR_RamTest(reinterpret_cast<int*>(ramAddr));
     if(ret != NULL){
         ADIN_LOG(__ERROR) << ret;
     }
     return ret;
 }

}
