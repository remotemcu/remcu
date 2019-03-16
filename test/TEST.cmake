
set(MCU_TYPE TEST)

set(MCU_LIB_NAME TEST_LIB)

set(TARGET_REMCU_OS_NAME testing)

set(MCU_MAJOR_VERSION_LIB 1)

set(MCU_MINOR_VERSION_LIB 00)

set(FULL_NAME_MCU_LIB ${MCU_TYPE}-${MCU_LIB_NAME}-${MCU_MAJOR_VERSION_LIB}-${MCU_MINOR_VERSION_LIB} )

set(CONF_SOURCE ${CMAKE_CURRENT_LIST_DIR}/conf.cpp)

set(MCU_OBJECT_FILE ${CMAKE_CURRENT_LIST_DIR}/build/${MCU_TYPE}.adin.o)

add_custom_target( MCU_LIB
	#OUTPUT ${MCU_OBJECT_FILE}
                   COMMAND make all
                   REMCU_PATH_MK=${REMCU_PATH_MK}
                   OUTPUT=${MCU_OBJECT_FILE}
                   CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                   COMMENT "---------------- Generating ADIN IR ----------------"
)

set(TARGET_DIR_PATH ${CMAKE_CURRENT_LIST_DIR})

function(copy_target_files INCLUDE_DIR_PATH README_DIR_PATH UTIL_DIR_PATH)
	message(STATUS "Copy Header to: ${INCLUDE_DIR_PATH}")

	FILE(GLOB COPY_SOURCE_FROM_LIB "${TARGET_DIR_PATH}/*.h")
	file(COPY ${COPY_SOURCE_FROM_LIB}
		DESTINATION  ${INCLUDE_DIR_PATH}
		)

    file(INSTALL ${TARGET_DIR_PATH}/platform/devices/MK64F12/include/
    	DESTINATION ${INCLUDE_DIR_PATH}/MK64F12/include/
    	FILES_MATCHING PATTERN "*.h"
    	)
    file(INSTALL ${TARGET_DIR_PATH}/platform/hal/src/sim/MK64F12/
    	DESTINATION ${INCLUDE_DIR_PATH}/src/sim/MK64F12/
    	FILES_MATCHING PATTERN "*.h"
    	)
    file(INSTALL ${TARGET_DIR_PATH}/platform/system/src/clock/MK64F12/
    	DESTINATION ${INCLUDE_DIR_PATH}/src/clock/MK64F12/
    	FILES_MATCHING PATTERN "*.h"
    	)
    file(COPY "${TARGET_DIR_PATH}/README.txt"
		DESTINATION  ${README_DIR_PATH}
		)
endfunction()