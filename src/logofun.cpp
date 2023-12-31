#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#include "ExportsCPP.h"
#include "logofun.h"
#include "obusfaction.h"

using namespace std;

namespace remcu {

//deprecated
static void printLicense1(){
    unsigned char s[] =
        {

            0x6e, 0xf5, 0x22, 0x35, 0xf1, 0xe7, 0xef, 0x76,
            0x6d, 0x30, 0xe9, 0x6d, 0x6d, 0x2a, 0xae, 0x63,
            0xa1, 0xa3, 0x5e, 0x28, 0x25, 0x25, 0xdb, 0x69,
            0x99, 0xdc, 0x45, 0x59, 0x13, 0xa8, 0x5d, 0xd7,
            0xd, 0xcd, 0x4e, 0xe4, 0x7d, 0x44, 0x6e, 0x8e,
            0xb7, 0x17, 0xd3, 0xd1, 0xd6, 0x8c, 0x95, 0xc6,
            0x5c, 0x15, 0x9a, 0x1b, 0x61, 0x58, 0x34, 0x74,
            0x83, 0xc1, 0xd4, 0x8, 0xa, 0x6, 0x5, 0x4b,
            0xdc, 0x61, 0xe7, 0x95, 0x55, 0xaa, 0x68, 0x61,
            0xde, 0x4e, 0xa0, 0x54, 0x94, 0x5d, 0x1d, 0x1f,
            0x24, 0x73, 0x2f, 0xb1, 0xb9, 0x75, 0xb7, 0xeb,
            0x46, 0xeb, 0x69, 0xb7, 0xad, 0x81, 0xe1, 0xec,
            0x7e, 0x7f, 0xbe, 0xea, 0xba, 0x9d, 0x74, 0x60,
            0xc6, 0x72, 0x87, 0x82, 0x87, 0x62, 0x35, 0xe8,
            0x4a, 0x4a, 0x36, 0xab, 0xe, 0xe1, 0xce, 0xaf,
            0xde, 0x93, 0x54, 0xe4, 0xa0, 0xa0, 0xd9, 0x19,
            0xc, 0x5a, 0xd2, 0x42, 0x7a, 0x15, 0xcc, 0xd8,
            0xfa, 0xf, 0x8d, 0x4b, 0x49, 0xc1, 0x8d, 0xd,
            0x84, 0x86, 0x42, 0x8, 0x0, 0xcb, 0xfb, 0x8,
            0x96, 0x3a, 0x0, 0x7, 0x7d, 0x22, 0x74, 0x1,
            0x0, 0xef, 0xef, 0x8b, 0x8b, 0xa9, 0xe, 0xae,
            0xb5, 0xf3, 0x55, 0xf5, 0xe7, 0xb7, 0x96, 0x17,
            0x3c, 0x20, 0x7a, 0x1a, 0xdd, 0x54, 0x9f, 0x73,
            0x22, 0x7e, 0xa1, 0xa1, 0xe9, 0xeb, 0xe5, 0xff,
            0x6d, 0x85, 0xbc, 0x43, 0x41, 0xb7, 0x81, 0x46,
            0x0, 0x39, 0x3d, 0x3a, 0xef, 0x3d, 0x7e, 0xfe,
            0x13, 0xd7, 0xe3, 0x36
        };

    for (unsigned int m = 0; m < sizeof(s); ++m)
    {
        unsigned char c = s[m];
        c = -c;
        c -= 0x40;
        c ^= 0xbb;
        c -= 0x97;
        c ^= m;
        c = ~c;
        c ^= 0xd8;
        c = ~c;
        c = -c;
        c += 0x9a;
        c ^= 0x70;
        c = (c >> 0x6) | (c << 0x2);
        c -= 0x79;
        c ^= 0x41;
        c += m;
        s[m] = c;
    }

    printf("%s\n", s);
}

static void printLine(){
    unsigned char s[] =
        {

            0x69, 0x31, 0x69, 0x81, 0x69, 0x31, 0x69, 0xa0,
            0x27, 0xef, 0xa7, 0x7f, 0x27, 0xf0, 0xa8, 0xe0,
            0x65, 0x2d, 0x65, 0x7d, 0x65, 0x2d, 0x65, 0xa0,
            0xa7, 0x70, 0x28, 0x7f, 0xa8, 0x70, 0x28, 0x60,
            0x6a, 0x32, 0x6a, 0x82, 0x6a, 0x32, 0x6a, 0xa1,
            0x2a, 0xf2, 0xaa, 0x82, 0x2a, 0xe7, 0x9f, 0xd7,
            0x5e, 0x26, 0x5e, 0x76, 0x5e, 0x26, 0x5e, 0x95,
            0x9f, 0x68, 0x20, 0x76, 0xa0, 0x68, 0x20, 0x58,
            0x59, 0x31, 0x59, 0x3a
        };

    for (unsigned int m = 0; m < sizeof(s); ++m)
    {
        unsigned char c = s[m];
        c = -c;
        c = ~c;
        c += 0x6c;
        c = (c >> 0x5) | (c << 0x3);
        c ^= m;
        c += m;
        c = (c >> 0x5) | (c << 0x3);
        c ^= m;
        c = ~c;
        c -= 0x67;
        c ^= m;
        c = -c;
        c = ~c;
        c -= 0xdf;
        c ^= 0xa9;
        s[m] = c;
    }

    printf("%s\n", s);
}


std::string getVersion(){

#ifndef _FULL_VERSION_NAME_
    #error unknown _FULL_VERSION_NAME_
#endif
    return
#ifndef NDEBUG
    string("!DEBUG!-") +
#endif
        string(_S_(_FULL_VERSION_NAME_));
}


void printLogo(){
#if (defined(_WIN32) || defined(WIN32) || defined(WIN64) || defined(_WIN64))
    cout << "########  ######## ##     ##  #######  ######## ########          \n##     ## ##       ###   ### ##     ##    ##    ##                \n##     ## ##       #### #### ##     ##    ##    ##                \n########  ######   ## ### ## ##     ##    ##    ######            \n##   ##   ##       ##     ## ##     ##    ##    ##                \n##    ##  ##       ##     ## ##     ##    ##    ##                \n##     ## ######## ##     ##  #######     ##    ########          \n   ##     ##  ######  ##     ##                                   \n   ###   ### ##    ## ##     ##                                   \n   #### #### ##       ##     ##                                   \n   ## ### ## ##       ##     ##                                   \n   ##     ## ##       ##     ##                                   \n   ##     ## ##    ## ##     ##                                   \n   ##     ##  ######   #######                                    \n ######   #######  ##    ## ######## ########   #######  ##       \n##    ## ##     ## ###   ##    ##    ##     ## ##     ## ##       \n##       ##     ## ####  ##    ##    ##     ## ##     ## ##       \n##       ##     ## ## ## ##    ##    ########  ##     ## ##       \n##       ##     ## ##  ####    ##    ##   ##   ##     ## ##       \n##    ## ##     ## ##   ###    ##    ##    ##  ##     ## ##       \n ######   #######  ##    ##    ##    ##     ##  #######  ######## \n\n"
         << endl;
#else
    cout << "\n██████╗ ███████╗███╗   ███╗ ██████╗ ████████╗███████╗        \n██╔══██╗██╔════╝████╗ ████║██╔═══██╗╚══██╔══╝██╔════╝        \n██████╔╝█████╗  ██╔████╔██║██║   ██║   ██║   █████╗          \n██╔══██╗██╔══╝  ██║╚██╔╝██║██║   ██║   ██║   ██╔══╝          \n██║  ██║███████╗██║ ╚═╝ ██║╚██████╔╝   ██║   ███████╗        \n╚═╝  ╚═╝╚══════╝╚═╝     ╚═╝ ╚═════╝    ╚═╝   ╚══════╝        \n                                                             \n            ███╗   ███╗ ██████╗██╗   ██╗                     \n            ████╗ ████║██╔════╝██║   ██║                     \n            ██╔████╔██║██║     ██║   ██║                     \n            ██║╚██╔╝██║██║     ██║   ██║                     \n            ██║ ╚═╝ ██║╚██████╗╚██████╔╝                     \n            ╚═╝     ╚═╝ ╚═════╝ ╚═════╝                      \n                                                             \n ██████╗ ██████╗ ███╗   ██╗████████╗██████╗  ██████╗ ██╗     \n██╔════╝██╔═══██╗████╗  ██║╚══██╔══╝██╔══██╗██╔═══██╗██║     \n██║     ██║   ██║██╔██╗ ██║   ██║   ██████╔╝██║   ██║██║     \n██║     ██║   ██║██║╚██╗██║   ██║   ██╔══██╗██║   ██║██║     \n╚██████╗╚██████╔╝██║ ╚████║   ██║   ██║  ██║╚██████╔╝███████╗\n ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝ ╚══════╝"
         << endl;
#endif

    //deprecated
#if 0
    printLine();
    printLicense();
    printLine();
#endif
    cout << "Version: " << getVersion() << endl;

}

} //namespace
