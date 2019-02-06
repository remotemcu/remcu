#ifndef DEFINES_H
#define DEFINES_H

#if (defined(_MSC_VER))

#define DECL_UNUSED
#define DECL_BODY_VIRTUAL_FUNCTION {return false;}

#else

#define DECL_UNUSED __attribute__((unused))
#define DECL_BODY_VIRTUAL_FUNCTION ;

#endif

#endif // DEFINES_H
