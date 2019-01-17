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



using namespace ocd_lib;
using namespace std;


int main(int argc, char** argv)
{
    connect2OpenOcd("192.168.0.111",6666);

   cout << "store2RemoteAddr : " <<  store2RemoteAddr(0x20000008, 0xFF1, 32) << endl;

   cout << "---------------------------" << endl;

   llvm_pass_t v;
   loadFromRemoteAddr(0x20000008, v, 32);
   cout << "loadFromRemoteAddr : " << v << endl;

 cout << "---------------------------" << endl;

 loadFromRemoteAddr(0x20000000, v, 32);

 cout << "loadFromRemoteAddr : " << v << endl;

 cout << "---------------------------" << endl;

 store2RemoteAddr(0x2000000C, 111, 32);

 cout << "---------------------------" << endl;

 loadFromRemoteAddr(0x2000000C, v, 32);

 cout << "loadFromRemoteAddr : " << v << endl;

 cout << "---------------------------" << endl;


}
