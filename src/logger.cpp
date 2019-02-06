#include <stdio.h>
#include <stdarg.h>

#include "logger.h"


namespace adin {

LevelDebug Log::gLevel = _ERROR;


void setVerboseLevel(LevelDebug level){
    Log::setGLevel(level);
}

const char * Log::getNameOfLevel(const LevelDebug level){

    switch (level) {
    case LevelDebug::_ERROR:
        return "(ERROR)";
    case LevelDebug::_INFO:
        return "(INFO)";
    case LevelDebug::_WARNING:
        return "(WARNING)";
    case LevelDebug::_DEBUG:
        return "(DEBUG)";
    case LevelDebug::_ALL_LOG:
        return "(DEBUG)";
    default:
        return "! It's very strangly !";
    }
}


int Log::loggerf (const LevelDebug level, const std::string &fileName,
             const std::string &funcName, const int line,
             const char *__restrict __format, ...){

    if (level > gLevel)
        return 0;

    std::cout << getNameOfLevel(level) << "$"
              << fileName << "/" << funcName << ":" << line << ": ";

    va_list args;
    va_start(args, __format);

    const int n = vprintf(__format, args);

    va_end(args);

    return n;
}


} //namespace
