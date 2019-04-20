/**
 * @cond
 *********************************************************************************************************************
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with
 * Infineon Technologies AG dave@infineon.com).
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2019-05-25:
 *     - Initial version
 *
 * @endcond
 */

#ifndef REMCU_H
#define REMCU_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef __EXPORT
#define __EXPORT
#endif

#ifndef REMCULIB_DLL_API
  #if defined(_WIN32) || defined(WIN32)
    #define REMCULIB_DLL_API __declspec(dllimport)
  #else 
    #define REMCULIB_DLL_API
  #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum ResetType {
    __RUN = 0,
    __HALT,
};

enum LevelDebug { __ERROR = 0,
                  __WARNING, __INFO,
                  __DEBUG, __ALL_LOG };

typedef void (*ErrorSignalFunc)();

#define _SIZE_ONE_MEMPCY 33

#define _DEFAULT_TIMEOUT_SEC 3

REMCULIB_DLL_API bool remcu_connect2OpenOCD(const char* host, const uint16_t port,
                      const int timeout_sec);

REMCULIB_DLL_API bool remcu_connect2GDB(const char* host, const uint16_t port,
                       const int timeout_sec);

REMCULIB_DLL_API bool remcu_disconnect();

REMCULIB_DLL_API bool remcu_is_connected();

REMCULIB_DLL_API const char * remcu_getVersion();

REMCULIB_DLL_API bool remcu_resetRemoteUnit(const enum ResetType type);

REMCULIB_DLL_API void remcu_setVerboseLevel(const enum LevelDebug level);

REMCULIB_DLL_API void remcu_setErrorSignalFunc(ErrorSignalFunc callback);

REMCULIB_DLL_API size_t remcu_getErrorCout();

REMCULIB_DLL_API void remcu_clearErrorCount();

REMCULIB_DLL_API bool remcu_store2mem(const uintptr_t addr, const uint8_t* sink, const size_t size);

REMCULIB_DLL_API bool remcu_loadFrMem(const uintptr_t addr, const size_t size, uint8_t* dist);

REMCULIB_DLL_API const char* remcu_targetRAMtest(uintptr_t ramAddr);

#ifdef __cplusplus
} //extern "C"
#endif

#include "device_defines.h"

#endif // REMCU_H
