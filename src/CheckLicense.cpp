#include <iostream>
#include <fstream>

#include "picosha2.h"
#include "logger.h"
#include "license.h"

using namespace std;


#ifndef _LICENSE_TYPE_
    #error License not set!
#endif


namespace remcu {

static const char _TOKEN = '#';
static const size_t _MAX_SIZE_TEXT = 10000;
static const size_t _MIN_SIZE_TEXT = 100;
static const size_t _SIZE_SIGN = 32*2 + 1;

static const char * license_type = _LICENSE_TYPE_;



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
            cout << line << endl;
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

void showLicesne(){
    const string LT(license_type);
    if( LT == string(_S_("EDU")) ){
        cout << _S_("\n******************************************************************* \n\nIt is Education license. \n \nREMCU lib can freely be used for non-profit educational purposes without any  \nlimitation. Simply download and install it and start development of your new  \nprojects. No registration or activation process is required. \n \nIn cooperation with organization, REMCU Lib can be made available free of  \ncharge for commercial use.\nemail: license@remotemcu.com \n See REMCU_LICENSE.txt file\n\n******************************************************************* \n")
             << endl;
    } else if( LT == string(_S_("CMT")) ){
        cout << _S_("\n******************************************************************* \n\nIt is a Community license. \n \nREMCU lib can freely be used for non-profit educational purposes without any  \nlimitation. Simply download and install it and start development of your new  \nprojects. No registration or activation process is required. \n \nIf you are an individual working on your own devices to sell or for any other  \npurpose, you may use the software to develop and test it without any limitation. \n \nIn cooperation with organization, REMCU Lib can be made available free of  \ncharge for commercial use.\nemail: license@remotemcu.com \nsee REMCU_LICENSE.txt file\n\n******************************************************************* \n")
             << endl;
    } else {
        cout << _S_("Commercial Licesne") << endl;
    }
}

} //namespace
