#ifndef DEFINES_H
#define DEFINES_H

#define DECL_UNUSED __attribute__((unused))

#if (defined(_WIN32) || defined(WIN32) || defined(WIN64) || defined(_WIN64))
#define REMCULIB_DLL_API __declspec(dllexport)
#else
#define REMCULIB_DLL_API __attribute__ ((visibility("default")))
#endif

#endif // DEFINES_H
