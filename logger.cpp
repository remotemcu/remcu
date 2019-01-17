#include "logger.h"
#include <stdio.h>
#include <stdarg.h>

namespace adin {

LevelDebug Log::gLevel = _ERROR;


void setVerboseLevel(LevelDebug level){
    Log::setGLevel(level);
}


int Log::loggerf (const LevelDebug level, const std::string &fileName,
             const std::string &funcName, const int line,
             const char *__restrict __format, ...){

    if (level > gLevel)
        return 0;

    std::cout << fileName << "/" << funcName << ":" << line << ": ";

    va_list args;
    va_start(args, __format);

    const int n = vprintf(__format, args);

    va_end(args);

    return n;
}


} //namespace
