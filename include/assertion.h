#ifndef ASSERTION_H
#define ASSERTION_H

#include <assert.h>

#include "logger.h"

namespace remcu {


#ifdef NDEBUG

    #define assert_1message(expr, msg)                                      \
      if(expr == false){                                                         \
            ADIN_LOG(__ERROR) << _S_(msg);                                        \
            return false;                                                   \
        }

    #define assert_printf(expr, msg,...)                                      \
      if(expr == false){                                                      \
            ADIN_PRINTF(__ERROR, msg, __VA_ARGS__);                      \
            return false;                                                   \
        }                                                                   \

#else

    #define assert_1message(expr, msg)                                      \
      if(expr){                                                             \
            ADIN_PRINTF(__ALL_LOG,"utv: %s\n", #expr);                            \
        } else {                                                            \
            ADIN_LOG(__ERROR) << _S_(msg);                                        \
            return false;                                                   \
        }

    #define assert_printf(expr, msg,...)                                      \
      if(expr){                                                             \
            ADIN_PRINTF(__ALL_LOG,"utv: %s\n", #expr);                            \
        } else {                                                            \
            ADIN_PRINTF(__ERROR, msg, __VA_ARGS__);                          \
            return false;                                                   \
        }                                                                   \

#endif

} //namespace

#endif
