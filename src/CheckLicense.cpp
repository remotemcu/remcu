#include <iostream>
#include <fstream>

#include "picosha2.h"
#include "logger.h"
#include "license.h"

using namespace std;


#ifndef LICENSE_TYPE
    #error License not set!
#endif


namespace remcu {

static const char _TOKEN = '#';
static const size_t _MAX_SIZE_TEXT = 10000;
static const size_t _MIN_SIZE_TEXT = 100;
static const size_t _SIZE_SIGN = 32*2 + 1;

static const char * license_type = LICENSE_TYPE;



static size_t setErrorBit(ErrorLicense_Type flag){
    return 1 << flag;
}

size_t checkLicense(){
    size_t error = 0;
    string text;
    string line;
    string sign;
    ifstream rfile;
    bool flag_token = false;
    rfile.open(_S_("REMCU_LICENSE.txt"));

    if (rfile.is_open()) {
        while (getline(rfile, line)) {
            cout << line << endl;
            if((line.size() > 0) && (line.at(0) == _TOKEN)){
                flag_token = true;
                sign.assign(line);
                break;
            }
            text.append(line);
            if(text.size() > _MAX_SIZE_TEXT){
                error |= setErrorBit(ErrorLicense_Type::_BIG_SIZE);
                break;
            }
        }
        rfile.close();
    } else {
        error |= setErrorBit(ErrorLicense_Type::_CANT_OPEN_FILE);
    }

    if(text.size() < _MIN_SIZE_TEXT){
        error |= setErrorBit(ErrorLicense_Type::_SMALL_SIZE);
    }

    if(flag_token == false){
        error |= setErrorBit(ErrorLicense_Type::_NOT_TOKEN);
    }

    if(sign.size() != _SIZE_SIGN){
        error |= setErrorBit(ErrorLicense_Type::_WRONG_SING_SIZE);
    }

    if(error != ErrorLicense_Type::_NO_ERROR){
        return error;
    }

    //cout << "buf: '" << text << "'" << endl;

    text += string(license_type);

    const string hash_hex_str = picosha2::hash256_hex_string(text);

    //cout << "hash_hex_str: '" << hash_hex_str << "'" << endl;

    //cout << "read hash: '" << line << "'" << endl;

    if(strncmp(sign.data()+1, hash_hex_str.data(), hash_hex_str.size()) != 0){
        error |= setErrorBit(ErrorLicense_Type::_WRONG_SIZE);
    }

    if(error != ErrorLicense_Type::_NO_ERROR){
        return error;
    }

    return ErrorLicense_Type::_NO_ERROR;
}

} //namespace
