#include <iostream>
#include <fstream>

#include "picosha2.h"
#include "logger.h"

using namespace std;

namespace remcu {

static const char _TOKEN = '#';
static const size_t _MAX_SIZE_TEXT = 10000;
static const size_t _MIN_SIZE_TEXT = 100;
static const size_t _SIZE_SIGN = 32*2 + 1;

enum error_check_type{
    _NO_ERROR = 0,
    _CANT_OPEN_FILE,
    _BIG_SIZE,
    _SMALL_SIZE,
    _NOT_TOKEN,
    _WRONG_SING_SIZE,
    _WRONG_SIZE
};

static size_t setErrorBit(error_check_type flag){
    return 1 << flag;
}

bool checkLicense(){
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
                error |= setErrorBit(_BIG_SIZE);
                break;
            }
        }
        rfile.close();
    } else {
        error |= setErrorBit(_CANT_OPEN_FILE);
    }

    if(text.size() < _MIN_SIZE_TEXT){
        error |= setErrorBit(_SMALL_SIZE);
    }

    if(flag_token == false){
        error |= setErrorBit(_NOT_TOKEN);
    }

    if(sign.size() != _SIZE_SIGN){
        error |= setErrorBit(_WRONG_SING_SIZE);
    }

    if(error != _NO_ERROR){
        ADIN_LOG(__ERROR) << _S_("license err: '") << error;
        return false;
    }

    cout << "buf: '" << text << "'" << endl;

    const string hash_hex_str = picosha2::hash256_hex_string(text);

    cout << "hash_hex_str: '" << hash_hex_str << "'" << endl;

    cout << "read hash: '" << line << "'" << endl;

    if(strncmp(sign.data()+1, hash_hex_str.data(), hash_hex_str.size()) != 0){
        error |= setErrorBit(_WRONG_SIZE);
    }

    if(error != _NO_ERROR){
        ADIN_LOG(__ERROR) << _S_("license err: '") << error;
    }

    return true;
}

} //namespace
