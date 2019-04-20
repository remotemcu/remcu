#ifdef REMCU_EXPORTS_SYMBOL_ENTER
    #if (defined(_WIN32) || defined(WIN32) || defined(WIN64) || defined(_WIN64))
        #pragma clang attribute pop
    #endif
    #undef REMCU_EXPORTS_SYMBOL_ENTER
#else
    #error "REMCU_EXPORTS_SYMBOL_ENTER absent"
#endif
