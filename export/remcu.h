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
 * RemoteLabX remotemcu.com (remcu@remotemcu.com)
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2019-06-15:
 *     - Initial version
 *
 * @endcond
 */

#ifndef REMCU_H
#define REMCU_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

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

#define _MEM_OPERATION_SIZE 33

#define _DEFAULT_TIMEOUT_SEC 3

/**
 * @brief remcu_connect2OpenOCD
 * The function is used to create a connection to the OpenOCD server destination
 * @param host - ip of OpenOCD server "XXX.XXX.XXX.XXX" or "localhost"
 * @param port - port of OpenOCD server. Default is 6666
 * @param timeout_sec - An attempt to connect timed out without establishing a connection.
 * @return If no error occurs, function returns true
 */
REMCULIB_DLL_API bool remcu_connect2OpenOCD(const char* host, const uint16_t port,
                      const int timeout_sec);

/**
 * @brief remcu_connect2GDB
 * The function is used to create a connection to the GDB server destination
 * @param host - ip of GDB server "XXX.XXX.XXX.XXX" or "localhost"
 * @param port - port of GDB server. Default of OpenOCD is 3333
 * @param timeout_sec - An attempt to connect timed out without establishing a connection.
 * @return If no error occurs, function returns true
 */
REMCULIB_DLL_API bool remcu_connect2GDB(const char* host, const uint16_t port,
                       const int timeout_sec);

/**
 * @brief remcu_disconnect
 * Closes the connection to the debug server(OpenOCD or GDB).
 * @return true only if an existing connection was successfully closed.
 */
REMCULIB_DLL_API bool remcu_disconnect();

/**
 * @brief remcu_isConnected
 * @return Returns true if an attempt to connect is successful
 */
REMCULIB_DLL_API bool remcu_isConnected();

/**
 * @brief remcu_getVersion
 * @return verson of REMCU lib. Don't free the pointer after use!
 */
REMCULIB_DLL_API const char * remcu_getVersion();

/**
 * @brief remcu_resetRemoteUnit
 * Performs as hard a reset as possible
 * @param The parameter specifies what should happen after the reset
 * - __RUN Let the target run
 * - __HALT Immediately halt the target
 * @return If no error occurs, function returns true
 */
REMCULIB_DLL_API bool remcu_resetRemoteUnit(const enum ResetType type);

/**
 * @brief remcu_setVerboseLevel
 * The function sets the verbose level. There are several levels of verboseness one can choose from.
 * @param level
 * level 0 is almost silent, producing only necessary messages
 * level 1 is good for most purposes, where as,
 * level 2 is good when developing a new pipeline.
 * level 3 is good for debugging, especially when getting un-expected results.
 */
REMCULIB_DLL_API void remcu_setVerboseLevel(const enum LevelDebug level);

/**
 * @brief remcu_getErrorCount
 * @return the qty of errors that occurred during the last operations after start or call remcu_clearErrorCount
 */
REMCULIB_DLL_API size_t remcu_getErrorCount();

/**
 * @brief remcu_clearErrorCount
 * clear counter of errors
 */
REMCULIB_DLL_API void remcu_clearErrorCount();

/**
 * @brief remcu_store2mem
 * Copies count bytes from the local object pointed to by src to the remote MCU memory to by RAM address
 * @param addr - RAM MCU address to copy to. Example: it may be 0x20000000 for ARM cortex M* core
 * @param src - pointer to the memory location to copy from
 * @param count - number of bytes to copy. The param has to be no more than _MEM_OPERATION_SIZE !
 * @return If no error occurs, function returns true
 */
REMCULIB_DLL_API bool remcu_store2mem(const uintptr_t addr, const uint8_t* src, const size_t count);

/**
 * @brief remcu_loadFrMem
 * Copies count bytes from the remote MCU memory to by RAM address to the object pointed to by dest.
 * @param addr - RAM MCU address to copy from. Example: it may be 0x20000000 for ARM cortex M* core
 * @param count - number of bytes to copy. The param has to be no more than _MEM_OPERATION_SIZE !
 * @param dest - pointer to the memory location to copy to
 * @return If no error occurs, function returns true
 */
REMCULIB_DLL_API bool remcu_loadFrMem(const uintptr_t addr, const size_t count, uint8_t* dest);

/**
 * @brief remcu_targetRAMtest
 * Performs RAM test of remote MCU
 * @param ramAddr - start address of RAM.
 * Example: it may be 0x20000000 for ARM cortex M* core
 * @return If no error occurs, the function returns NULL
 * else the function returns error message (char array)
 * Don't free the pointer after use!
 */
REMCULIB_DLL_API const char* remcu_targetRAMtest(uintptr_t ramAddr);

#ifdef __cplusplus
} //extern "C"
#endif

#include "device_defines.h"

#endif // REMCU_H
