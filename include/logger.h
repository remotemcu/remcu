#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

#include "exports.h"

namespace remcu {


class Log {
    static LevelDebug gLevel;

    LevelDebug cLevel;

    static const char * getNameOfLevel(const LevelDebug);

public:
    Log(const LevelDebug level, const std::string &fileName,
        const std::string &funcName, const int line) {
        cLevel = level;
        if (cLevel <= gLevel)
            std::cout << getNameOfLevel(level) << "$"
                      << fileName
                      << "/" << funcName << ":"
                      << line << ": ";
    }

    Log(const LevelDebug level, const int line) {
        cLevel = level;
        if (cLevel <= gLevel)
            std::cout << getNameOfLevel(level) << "$"<< line << ": ";
    }

    template <class T> Log &operator<<(const T &v) {
        if (cLevel <= gLevel)
            std::cout << v;
        return *this;
    }

    ~Log() {
        if (cLevel <= gLevel)
            std::cout << std::endl;
    }

    static void setGLevel(const LevelDebug level) { gLevel = level; }

    static int loggerf (const LevelDebug level, const std::string &fileName,
                 const std::string &funcName, const int line,
                 const char *__restrict __format, ...);

    static int loggerf (const LevelDebug level, const int line,
                       const char *__restrict __format, ...);
};


} //namespace



#ifdef _DEBUG_BUILD_

#define ADIN_LOG(LEVEL) Log((LEVEL), __FILE__, __FUNCTION__, __LINE__)
#define ADIN_PRINTF(LEVEL,F__,...) Log::loggerf((LEVEL), __FILE__, __FUNCTION__, __LINE__, F__, __VA_ARGS__)

#else

#define ADIN_LOG(LEVEL) Log((LEVEL), __LINE__)
#define ADIN_PRINTF(LEVEL,F__,...) Log::loggerf((LEVEL), __LINE__, F__, __VA_ARGS__)

#endif


#endif // LOGGER_H
