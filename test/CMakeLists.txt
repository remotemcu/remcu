cmake_minimum_required (VERSION 3.5)

project(Test CXX)

set(MCU_TYPE TEST)

set(MCU_LIB_NAME TEST_LIB)

set(TARGET_REMCU_OS_NAME testing)

set(MCU_MAJOR_VERSION_LIB 1)

set(MCU_MINOR_VERSION_LIB 00)

set(FULL_NAME_MCU_LIB ${MCU_TYPE}-${MCU_LIB_NAME}-${MCU_MAJOR_VERSION_LIB}-${MCU_MINOR_VERSION_LIB} )

set(CONF_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/conf.cpp)

set(BUILD_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${FULL_NAME_MCU_LIB})

set(MCU_OBJECT_FILE ${BUILD_DIRECTORY}/${MCU_TYPE}.adin.o)

set(LICENSE_TYPE EDU)


set(CMAKE_MODULE_PATH ${REMCU_VM_PATH}/cmake)

include(McuCommon)

add_custom_target( MCU_LIB
                   COMMAND make all
                   MCU_UTIL_PATH=${MCU_UTIL_PATH}
                   TARGET_MK=${MCU_INCLUDE_MK_FILE}
                   OUTPUT=${MCU_OBJECT_FILE}
                   CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                   BUILD_DIRECTORY=${BUILD_DIRECTORY}
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                   COMMENT "---------------- Generating ADIN IR ----------------"
)

include(shared)

file(INSTALL ${CMAKE_CURRENT_SOURCE_DIR}/IrTest.h
      DESTINATION ${ALL_INCLUDE_DIR}
      )


add_executable(test_shared test.cpp)
target_include_directories(test_shared PRIVATE ${ALL_INCLUDE_DIR})
target_link_libraries(test_shared remcu)

#[[
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