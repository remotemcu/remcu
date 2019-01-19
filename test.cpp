/*
   client.cpp

   Test client for the tcpsockets classes. 

   ------------------------------------------

   Copyright (c) 2013 Vic Hargrave

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>
#include <assert.h>

#include "ocdcommand.h"
#include "netwrapper.h"
#include "AddressInterceptPass.h"
#include "adin.h"
#include "logger.h"
#include "assertion.h"

#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wextra"

int mcu();
void global();

using namespace adin;
using namespace std;

static bool callback(const char *__assertion, const char *__file,
                unsigned int __line, const char *__function){
    cout << "------------------ " << endl;
    return true;
}

llvm_pass_arg v = 0x87654321;

int main(int argc, char** argv)
{

    connect2OpenOcd("192.168.0.111",6666);


    ADIN_PRINTF(_DEBUG, "!!!!!!!!!!!!!!!!!\n",1);

    ADIN_LOG(_DEBUG) << "]]]]]]]]]]]]]]]]]]]]]]]]]]]";

    setVerboseLevel(_ALL_LOG);
    asser_1line(true);
    setVerboseLevel(_ERROR);
    setErrorFunction(callback);


    addInterceptAddress2Interval(0x20000000, 0x20000000 + 128*1024);
#if 0
    __adin_store_((llvm_pass_addr)0x20000000, 0, 32, 0);

    __adin_store_((llvm_pass_addr)0x20000000, 0x12345678, 32, 0);

    cout << "__adin_load_ : " << std::hex
         << __adin_load_((llvm_pass_addr)0x20000000, 8, 0) << endl;

    cout << "__adin_load_ : " << std::hex
         << __adin_load_((llvm_pass_addr)0x20000003, 16, 0) << endl;

    cout << "__adin_load_ : " << std::hex
         << __adin_load_((llvm_pass_addr)&v, 32, 0) << endl;
#endif
    mcu();
    global();

    asser_1line(false);
}
