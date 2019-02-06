#include <assert.h>

#include "assertion.h"


namespace adin {
#if 0
static ErrorFunction_t callback = NULL;

void setErrorFunction(const ErrorFunction_t fun){
    callback = fun;
}

bool existErrorCallback(){
    return callback != NULL;
}

bool __callback(const char *__assertion, const char *__file,
                const unsigned int __line, const char *__function){
    return (*callback)(__assertion, __file, __line, __function);
}

bool __assertionF (const char *__assertion, const char *__file,
                  const unsigned int __line, const char *__function){

    __assert_fail (__assertion, __file, __line, __function);
    return true;
}
#endif
} //namespace
