#ifndef NETWRAPPER_H
#define NETWRAPPER_H

#include <vector>
#include <string>
#include <stdint.h>


namespace adin {

#define TIMEOUT_OCD_RESPONSE 1

bool sendTCLMessage2OCD(char * buffer, size_t lenBuffer);

inline bool sendTCLMessage2OCD(std::string message){
    return sendTCLMessage2OCD((char*)message.c_str(), message.size());
}

bool receiveOCDResponse(char * buffer, size_t & lenBuffer, int timeout_sec = TIMEOUT_OCD_RESPONSE);

} //namespace

#endif // NETWRAPPER_H
