#include <string>

#include "logger.h"
#include "netwrapper.h"
#include "obusfaction.h"
#include "logofun.h"
#include "license.h"

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
                    const ServerType server = ServerType::_DUMMY_SERVVER,
                    const int timeout_sec = _DEFAULT_TIMEOUT_SEC);

bool connect2ServerLogo(const std::string host, const uint16_t port,
                        const ServerType server = ServerType::_DUMMY_SERVVER,
                        const int timeout_sec = _DEFAULT_TIMEOUT_SEC){

    const size_t ret = checkLicense();

    if(ret != ErrorLicense_Type::_NO_ERROR){
        ADIN_LOG(__ERROR) << _S_("license err: '") << ret;
        return false;
    }

    const bool success = connect2Server(host, port, server, timeout_sec);

    if(success){
        printLogo();
    } else {
        ADIN_LOG(__ERROR) << "Connecting failed!";
        ADIN_LOG(__ERROR) << "Please check server and try again...";
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
