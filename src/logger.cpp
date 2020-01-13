#include <stdio.h>
#include <stdarg.h>

#include "logger.h"


namespace remcu {

LevelDebug Log::gLevel = __ERROR;

static const char * license_type = _LICENSE_TYPE_;


void setVerboseLevel(LevelDebug level){
#ifdef NDEBUG
    const std::string LT(license_type);
    bool no_comm_license = false;
    no_comm_license |= LT == std::string(_S_("EDU"));
    no_comm_license |= LT == std::string(_S_("CMT"));
    if(no_comm_license){
        switch (level) {
        case LevelDebug::__ERROR:
        case LevelDebug::__WARNING:
        case LevelDebug::__INFO:
            break;
        case LevelDebug::__DEBUG:
        case LevelDebug::__ALL_LOG:
        default:
            level = LevelDebug::__INFO;
            break;
        }
    }
#endif
     Log::setGLevel(level);
}

const char * Log::getNameOfLevel(const LevelDebug level){
    static const std::string ERROR(_S_("(ERROR)"));
    static const std::string INFO(_S_("(INFO)"));
    static const std::string WARNING(_S_("(WARNING)"));
    static const std::string DEBUG(_S_("(DEBUG)"));

    switch (level) {
    case LevelDebug::__ERROR:
        return ERROR.data();
    case LevelDebug::__WARNING:
        return WARNING.data();
    case LevelDebug::__INFO:
        return INFO.data();
    case LevelDebug::__DEBUG:
        return DEBUG.data();
    case LevelDebug::__ALL_LOG:
        return DEBUG.data();
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
