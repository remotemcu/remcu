#include <string>

#include "logger.h"
#include "netwrapper.h"
#include "obusfaction.h"
#include "logofun.h"
#include "license.h"
#include "target.h"

using namespace std;

namespace remcu {

bool sendTCP(const char * buffer, const size_t lenBuffer);
bool receive(char * buffer, size_t & lenBuffer);


bool sendMessage2Server(const char * buffer, const size_t lenBuffer){
    const bool success = sendTCP(buffer, lenBuffer);

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
                    const ServerType server = ServerType::_DUMMY_SERVVER,
                    const int timeout_sec = _DEFAULT_TIMEOUT_SEC);

bool connect2ServerLogo(const std::string host, const uint16_t port,
                        const ServerType server = ServerType::_DUMMY_SERVVER,
                        const int timeout_sec = _DEFAULT_TIMEOUT_SEC){

    if(timeout_sec < 0){
        ADIN_LOG(__ERROR) << _S_("timeout can't be negative | current timeout : ") << timeout_sec;
        return false;
    }

    const bool success = connect2Server(host, port, server, timeout_sec);

    if(success){
        printLogo();
        clearConfig();
        setConfig();
    } else {
        ADIN_LOG(__ERROR) << _S_("Connecting failed!");
        ADIN_LOG(__ERROR) << _S_("Please check server and try again...");
    }

    return success;
}

bool connect2OpenOCD(const std::string host, const uint16_t port,
                     const int timeout_sec){
    return connect2ServerLogo(host, port, ServerType::_OPENOCD_SERVER, timeout_sec);
}

bool connect2GDB(const std::string host, const uint16_t port,
                 const int timeout_sec){
    return connect2ServerLogo(host, port, ServerType::_GDB_SERVER, timeout_sec);
}

} //namespace
