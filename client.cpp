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
#include "tcpconnector.h"

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("usage: %s <port> <ip>\n", argv[0]);
        exit(1);
    }

    int len;
    string message;
    char line[256] = "ocd_mdw 0x20000008\x1a\0";
    TCPConnector* connector = new TCPConnector();
    TCPStream* stream = connector->connect(argv[2], atoi(argv[1]));
    for (int i = 0; stream && i < 0xF; i+=1 ) {
        
      #if 1

      //int n = sprintf(line,"ocd_mdw 0x2000000%d\x1a\0", i);
      int n = sprintf(line,"ocd_mdw 0x2000000%x\x1a",i);      

        cout << "message: " << line  << endl;
        if(n<0)
          cout << "if(n<0)" << endl;

        stream->send(line, strlen(line));

        #else

        message = "ocd_mdw 0x20000008\x1a";
        printf("sent - %s\n", message.c_str());
        cout << "message:= " << message  << endl;
        stream->send(message.c_str(), message.size());
        //

        #endif


        len = stream->receive(line, sizeof(line));
        while(len == 1){
          cout << "-> " << endl;
          len = stream->receive(line, sizeof(line));

        }
        line[len] = 0;
        printf("received len %d - %s\n", len, line);
        
    }

    delete stream;
#if 0
    stream = connector->connect(argv[2], atoi(argv[1]));
    if (stream) {
        int n = sprintf("mdw 0x2000000%x\x1a", ;
        message = 
        stream->send(message.c_str(), message.size());
        printf("sent - %s\n", message.c_str());
        len = stream->receive(line, sizeof(line));
        line[len] = 0;
        printf("received - %s\n", line);
        delete stream;
    }
    #endif
    exit(0);
}
