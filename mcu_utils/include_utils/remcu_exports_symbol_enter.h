#ifdef REMCU_EXPORTS_SYMBOL_ENTER
    #error "REMCU_EXPORTS_SYMBOL_ENTER repeat"
#else
#define REMCU_EXPORTS_SYMBOL_ENTER
    #if (defined(_WIN32) || defined(WIN32) || defined(WIN64) || defined(_WIN64))
            #pragma clang attribute push (__declspec(dllexport), apply_to = function)
    #endif
#endif
