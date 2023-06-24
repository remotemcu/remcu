# Define our host system
SET(CMAKE_SYSTEM_NAME Linux)
# SET(CMAKE_SYSTEM_VERSION 1)

if(NOT DEFINED ENV{RASPBERRY_TOOL_PATH})
	message(FATAL_ERROR 
		"\n******************************************************\n"
		"udefined RASPBERRY_TOOL_PATH environment varibale, see: $ENV{RASPBERRY_TOOL_PATH} \n"
			"download raspberry toolchain from https://github.com/raspberrypi/tools \n"
			"cd <raspberry tool clone> \n"
			"git clone https://github.com/raspberrypi/tools \n"
			"export RASPBERRY_TOOL_PATH=<raspberry tool clone>/tools\n"
			"\n******************************************************\n"
			)
endif()

set(RASPBERRY_TOOL_PATH $ENV{RASPBERRY_TOOL_PATH})

# Define the cross compiler locations
SET(CMAKE_C_COMPILER   ${RASPBERRY_TOOL_PATH}/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER ${RASPBERRY_TOOL_PATH}/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)
# Define the sysroot path for the RaspberryPi distribution in our tools folder 
SET(CMAKE_FIND_ROOT_PATH ${RASPBERRY_TOOL_PATH}/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/arm-linux-gnueabihf/sysroot/)

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")
SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")

# Use our definitions for compiler tools
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# Search for libraries and headers in the target directories only
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

include(${CMAKE_CURRENT_LIST_DIR}/vars.cmake)

set(CMAKE_CXX_STANDARD 11)
include(${REMCU_VM_PATH}/cmake/common_linux_flags.cmake)

set(MCU_INCLUDE_MK_FILE ${REMCU_VM_PATH}/mcu_utils/linux.mk)


# set(STRIP_COMMAND ${RASPBERRY_TOOL_PATH}/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-strip)

set(TARGET_REMCU_OS_NAME Raspbian_armv6)

set(CROSS_MK_FILE ${REMCU_VM_PATH}/mcu_utils/arm-bcm2708.mk)

set(LICENSE_TYPE CMT)