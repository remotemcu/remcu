#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>

#include "assertion.h"
#include "netwrapper.h"
#include "logger.h"
#include "client.h"



using namespace std;


namespace adin {

vector<char> ClientBase::bufferSend(0x100,'\0');
vector<char> ClientBase::bufferReceiv(0x100,'\0');

bool ClientBase::connect(const std::string host, const uint16_t port, const int timeout_sec) const {
    return connectTCP(host, port, timeout_sec);
}

bool ClientBase::close() const {
    return closeTCP();
}

} //namespace
