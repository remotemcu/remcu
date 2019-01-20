#ifndef NETWRAPPER_H
#define NETWRAPPER_H

#include <vector>
#include <string>
#include <stdint.h>


namespace adin {

#define TIMEOUT_OCD_RESPONSE 1

bool connectTCP(std::string host, uint16_t port, int timeout_sec = 0);

bool closeTCP();

bool sendMessage2Server(char * buffer, size_t lenBuffer);

inline bool sendMessage2Server(std::string message){
    return sendMessage2Server((char*)message.c_str(), message.size());
}

bool receiveResponseFromServer(char * buffer, size_t & lenBuffer, int timeout_sec = TIMEOUT_OCD_RESPONSE);

} //namespace

#endif // NETWRAPPER_H
