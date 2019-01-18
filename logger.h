#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

#include "adin.h"

namespace adin {


class Log {
    static LevelDebug gLevel;

    LevelDebug cLevel;

public:
    Log(const LevelDebug level, const std::string &fileName,
        const std::string &funcName, const int line) {
        cLevel = level;
        if (cLevel <= gLevel)
            std::cout << fileName << "/" << funcName << ":" << line << ": ";
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
};



} //namespace

#define ADIN_LOG(LEVEL) Log((LEVEL), __FILE__, __FUNCTION__, __LINE__)

#define ADIN_PRINTF(LEVEL,F__,...) Log::loggerf((LEVEL), __FILE__, __FUNCTION__, __LINE__, F__, __VA_ARGS__)

#endif // LOGGER_H
