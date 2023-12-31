#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

#include "ExportsCPP.h"
#include "obusfaction.h"

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
                      << std::dec << line << ": ";
    }

    Log(const LevelDebug level, const int line) {
        cLevel = level;
        if (cLevel <= gLevel)
            std::cout << getNameOfLevel(level) << "$"<< std::dec << line << ": ";
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

#ifndef __BASENAME_SRC__
    #define __BASENAME_SRC__ "unknown"
#endif

#ifdef HIDE_SOURCE_INFO_IN_LOG
    #ifdef __BASENAME_SRC__
        #undef __BASENAME_SRC__
    #endif

    #define __BASENAME_SRC__ "#"

    #ifdef __FUNCTION__
        #undef __FUNCTION__
    #endif

    #define __FUNCTION__ "#"
#endif

#ifdef NDEBUG

#define ADIN_LOG(LEVEL) Log((LEVEL), _D_(__BASENAME_SRC__), _D_(__FUNCTION__), __LINE__)
#define ADIN_PRINTF(LEVEL,F__,...) Log::loggerf((LEVEL), _D_(__BASENAME_SRC__), _D_(__FUNCTION__), __LINE__, _S_(F__).c_str(), __VA_ARGS__)

#else

#define ADIN_LOG(LEVEL) Log((LEVEL), __BASENAME_SRC__, __FUNCTION__, __LINE__)
#define ADIN_PRINTF(LEVEL,F__,...) Log::loggerf((LEVEL), __BASENAME_SRC__, __FUNCTION__, __LINE__, F__, __VA_ARGS__)

#endif


#endif // LOGGER_H
