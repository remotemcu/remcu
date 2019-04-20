
#ifdef NDEBUG
#include <thread>

#include "logger.h"

using namespace std;

static
#include "vm_encript/vm.bin.h"

unsigned char * vm_blob_bin(){

    return blob_bin;
}

namespace remcu {

static char * encrypt(unsigned char * blob, size_t size){
    unsigned char * buffer = (unsigned char *)malloc(size);
    if(buffer == NULL){
        ADIN_LOG(__ERROR) << _S_("Error Unpack VM");
        return NULL;
    }
    memcpy(buffer, blob, size);
    for (unsigned int m = 0; m < sizeof(buffer); ++m)
    {
        unsigned char c = buffer[m];
        c = -c;
        c = ~c;
        c += 0x6c;
        c = (c >> 0x5) | (c << 0x3);
        c ^= m;
        c += m;
        c = (c >> 0x5) | (c << 0x3);
        c ^= m;
        c = ~c;
        c -= 0x67;
        c ^= m;
        c = -c;
        c = ~c;
        c -= 0xdf;
        c ^= 0xa9;
        buffer[m] = c;
    }
    free(buffer);

    return "OK";
}

static bool work_vm = false;

static void threadVM_Fun()
{
    const size_t sizeVM = sizeof (blob_bin);
    size_t index = 0;

    work_vm = true;

    if(encrypt(blob_bin, 3002) == NULL){
        ADIN_LOG(__ERROR) << _S_("BLOB VM");
        work_vm = false;
        return;
    }

    while (1) {

        if(index >= sizeVM){
            index = 0;
        }
        const size_t qty = (sizeVM - index)%463;
        encrypt(blob_bin + index, qty);
        index += qty;
        std::this_thread::sleep_for(2s);
    }
}

void startVM(const char * options){
    thread thread_VM(threadVM_Fun);
    if(options == nullptr)
        return;

    if(work_vm == false){
        ADIN_LOG(__DEBUG) << _S_("VM started");
        return;
    }

    ADIN_LOG(__DEBUG) << _S_("START VM");

    thread_VM.detach();
}

} //namespace

#else

namespace remcu {
void startVM(const char * options){
    (void)options;
}
} //namespace

#endif
