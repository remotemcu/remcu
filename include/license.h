#ifndef LICENSE_H
#define LICENSE_H

namespace remcu {

enum ErrorLicense_Type{
    _NO_ERROR = 0,
    _CANT_OPEN_FILE,
    _BIG_SIZE,
    _SMALL_SIZE,
    _NOT_TOKEN,
    _WRONG_SING_SIZE,
    _WRONG_SIZE
};

size_t checkLicense();

}


#endif // LICENSE_H
