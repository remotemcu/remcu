
if(NOT DEFINED MCU_OBJECT_FILE)
	message(FATAL_ERROR "******* $MCU_OBJECT_FILE is not defined! ********")
endif()

if(NOT DEFINED MCU_TYPE)
	message(FATAL_ERROR "******* $MCU_TYPE is not defined! ********")
endif()

if(NOT DEFINED CONF_SOURCE)
	message(FATAL_ERROR "******* $CONF_SOURCE is not defined! ********")
endif()

set(ALL_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/remcu_include)
set(ARCHIVE_ROOT_PAT ${CMAKE_CURRENT_BINARY_DIR})

set(REMCU_VM_BUILD_PATH build_remcu_object)

include(archive)

add_subdirectory(${REMCU_VM_PATH} ${REMCU_VM_BUILD_PATH})

set(MCU_MAKEFILE_PATH ${REMCU_VM_PATH}/Makefiles/common.mk)