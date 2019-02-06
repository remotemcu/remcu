#ifndef ASSERTION_H
#define ASSERTION_H

#include <assert.h>

#include "logger.h"

namespace adin {

#if 0
void __ASSERT__DEBUG(const char *);

bool existErrorCallback();

bool __callback(const char *__assertion, const char *__file,
                const unsigned int __line, const char *__function);

bool __assertionF (const char *__assertion, const char *__file,
                  const unsigned int __line, const char *__function);
#endif

# define assert_1message(expr, msg)                                      \
  if(expr){                                                             \
        ADIN_PRINTF(__ALL_LOG,"%s\n", #expr);                            \
    } else {                                                            \
        ADIN_LOG(__ERROR) << msg;                                        \
        return false;                                                   \
    }                                                                   \


# define assert_printf(expr, msg,...)                                      \
  if(expr){                                                             \
        ADIN_PRINTF(__ALL_LOG,"%s\n", #expr);                            \
    } else {                                                            \
        ADIN_PRINTF(__ERROR, msg, __VA_ARGS__);                          \
        return false;                                                   \
    }                                                                   \

} //namespace

#endif
