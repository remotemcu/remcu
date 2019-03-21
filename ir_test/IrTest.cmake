
set(BUILD_IR_TEST_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/IrTest)

set(IR_TEST_OBJECT_FILE ${BUILD_IR_TEST_DIRECTORY}/IrTest.adin.o)

#message(FATAL_ERROR "******* $IR_TEST_OBJECT_FILE ${IR_TEST_OBJECT_FILE} ********")

add_custom_target( IR_TEST_OBJ
                   COMMAND make all
                   MCU_UTIL_PATH=${MCU_UTIL_PATH}
                   TARGET_MK=${MCU_INCLUDE_MK_FILE}
                   OUTPUT=${IR_TEST_OBJECT_FILE}
                   CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                   BUILD_DIRECTORY=${BUILD_IR_TEST_DIRECTORY}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
                   COMMENT "---------------- Generating IR Test ----------------"
)

#[[
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
]]
