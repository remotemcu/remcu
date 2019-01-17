#include "logger.h"
#include <stdio.h>
#include <stdarg.h>

namespace adin {

LevelDebug Log::gLevel = _ERROR;


void setVerboseLevel(LevelDebug level){
    Log::setGLevel(level);
}


void Log::loggerf (const LevelDebug level, const std::string &fileName,
             const std::string &funcName, const int line,
             const char *__restrict __format, ...){

    if (level >= gLevel)
        return;

    std::cout << fileName << "/" << funcName << ":" << line << ": ";

    va_list args;
    va_start(args, __format);

    vprintf(__format, args);

    va_end(args);
}


} //namespace
