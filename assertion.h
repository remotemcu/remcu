#ifndef ASSERTION_H
#define ASSERTION_H

#include <assert.h>

#include "logger.h"

namespace adin {


void __ASSERT_DEBUG(const char *);

bool existErrorCallback();

bool __callback(const char *__assertion, const char *__file,
                unsigned int __line, const char *__function);

bool __assertionF (const char *__assertion, const char *__file,
                  unsigned int __line, const char *__function);



//ADIN_PRINTF(_ALL_LOG,"%s",#expr)
} //namespace

# define asser_1line(expr)							\
  ((expr)								\
   ? 	ADIN_PRINTF(_ALL_LOG,"%s\n", #expr)			\
   :  existErrorCallback()   \
    ?  __callback(#expr, __FILE__, __LINE__, __ASSERT_FUNCTION) \
    : __assertionF(#expr, __FILE__, __LINE__, __ASSERT_FUNCTION))    \


#endif // ASSERTION_H
