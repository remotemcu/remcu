#ifndef ASSERTION_H
#define ASSERTION_H

#include <assert.h>

#include "logger.h"

namespace adin {


void __ASSERT_DEBUG(const char *);

bool existErrorCallback();

bool __callback(const char *__assertion, const char *__file,
                const unsigned int __line, const char *__function);

bool __assertionF (const char *__assertion, const char *__file,
                  const unsigned int __line, const char *__function);



//ADIN_PRINTF(_ALL_LOG,"%s",#expr)
} //namespace

# define asser_1line(expr)                                              \
  if(expr){                                                             \
        ADIN_PRINTF(_ALL_LOG,"%s\n", #expr);                            \
    } else {                                                            \
       if(existErrorCallback()) {   \
          __callback(#expr, __FILE__, __LINE__, __ASSERT_FUNCTION);     \
        } else {                                                        \
         __assertionF(#expr, __FILE__, __LINE__, __ASSERT_FUNCTION);   \
        }                                                               \
        return false;                                                   \
    }                                                                   \

#endif // ASSERTION_H
