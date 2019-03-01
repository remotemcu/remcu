// add dummy blob

#ifdef NDEBUG
static
#include "dummy/dummy.bin.h"

unsigned char * dummy(){

    return blob_bin;
}

#endif
