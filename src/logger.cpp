#include <stdio.h>
#include <stdarg.h>

#include "logger.h"


namespace remcu {

LevelDebug Log::gLevel = __ERROR;


void setVerboseLevel(LevelDebug level){
    Log::setGLevel(level);
}

const char * Log::getNameOfLevel(const LevelDebug level){

    switch (level) {
    case LevelDebug::__ERROR:
        return "(ERROR)";
    case LevelDebug::__INFO:
        return "(INFO)";
    case LevelDebug::__WARNING:
        return "(WARNING)";
    case LevelDebug::__DEBUG:
        return "(DEBUG)";
    case LevelDebug::__ALL_LOG:
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
              << fileName << "/" << funcName << ":" << std::dec << line << ": ";

    va_list args;
    va_start(args, __format);

    const int n = vprintf(__format, args);

    va_end(args);

    return n;
}

int Log::loggerf (const LevelDebug level, const int line,
                 const char *__restrict __format, ...){

    if (level > gLevel)
        return 0;

    std::cout << getNameOfLevel(level) << "$" << std::dec << line << ": ";

    va_list args;
    va_start(args, __format);

    const int n = vprintf(__format, args);

    va_end(args);

    return n;
}


} //namespace
