#ifndef DEFINES_H
#define DEFINES_H

#define DECL_UNUSED __attribute__((unused))
#define DECL_BODY_VIRTUAL_FUNCTION ;

#if (defined(_WIN32) || defined(WIN32) || defined(WIN64) || defined(_WIN64))
#define __EXPORT __declspec(dllexport)
#else
#define __EXPORT __attribute__ ((visibility("default")))
#endif

#endif // DEFINES_H
