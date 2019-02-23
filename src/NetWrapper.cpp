#include <string>

#include "logger.h"
#include "netwrapper.h"
#include "obusfaction.h"


using namespace std;

namespace remcu {

bool send(const char * buffer, const size_t lenBuffer);
bool receive(char * buffer, size_t & lenBuffer);


bool sendMessage2Server(const char * buffer, const size_t lenBuffer){
    const bool success = send(buffer, lenBuffer);

    if(success == false){
        ADIN_LOG(__ERROR) << _S_("send message failed^");
    } else {
        const string buf(buffer, lenBuffer);
        ADIN_LOG(__DEBUG) << _S_("-> ") << lenBuffer << _S_(" : '") << buf << "'";
    }

    return success;
}

bool receiveResponseFromServer(char * buffer, size_t & lenBuffer){
    const bool success = receive(buffer, lenBuffer);

    if(success == false){
        ADIN_LOG(__ERROR) << _S_("receive message failed^");
    } else {
        const string buf(buffer, lenBuffer);
        ADIN_LOG(__DEBUG) << _S_("<- ") << lenBuffer << _S_(" : '") << buf << "'";
    }

    return success;
}

bool connect2Server(const std::string host, const uint16_t port,
                    const ServerType server = _DUMMY_SERVVER,
                    const int timeout_sec = _DEFAULT_TIMEOUT_SEC);

bool connect2OpenOCD(const std::string host, const uint16_t port,
                     const int timeout_sec){
    return connect2Server(host, port, _OPENOCD_SERVER, timeout_sec);
}

bool connect2GDB(const std::string host, const uint16_t port,
                 const int timeout_sec){
    return connect2Server(host, port, _GDB_SERVER, timeout_sec);
}

} //namespace
