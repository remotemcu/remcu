#ifndef NETWRAPPER_H
#define NETWRAPPER_H

#include <string>
#include <stdint.h>


namespace adin {

#define TIMEOUT_SEC_OCD_RESPONSE 3

bool connectTCP(const std::string host, const uint16_t port, const int timeout_sec = TIMEOUT_SEC_OCD_RESPONSE);

bool closeTCP();

bool sendMessage2Server(const char * buffer, const size_t lenBuffer);

inline bool sendMessage2Server(const std::string message){
    return sendMessage2Server((char*)message.c_str(), message.size());
}

bool receiveResponseFromServer(char * buffer, size_t & lenBuffer);

} //namespace

#endif // NETWRAPPER_H
