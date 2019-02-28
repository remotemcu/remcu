
if(NOT MCU_LIB_PATH)
	if(NOT ENV{MCU_LIB_PATH})
		message(FATAL_ERROR "******* $MCU_LIB_PATH is not defined! ********")
	else()
		set(MCU_LIB_PATH ENV{MCU_LIB_PATH})
	endif()
endif()


if(NOT MCU_NAME_LIB)
	if(NOT ENV{MCU_NAME_LIB})
		message(FATAL_ERROR "******* $MCU_NAME_LIB is not defined! ********")
	else()
		set(MCU_NAME_LIB ENV{MCU_NAME_LIB})
	endif()
endif()

add_definitions(-D_MCU_NAME_LIB_=\"${MCU_NAME_LIB}\")

if(NOT MCU_VERSION_LIB)
	if(NOT ENV{MCU_VERSION_LIB})
		message(FATAL_ERROR "******* $MCU_VERSION_LIB is not defined! ********")
	else()
		set(MCU_VERSION_LIB ENV{MCU_VERSION_LIB})
	endif()
endif()

add_definitions(-D_MCU_VERSION_LIB_=\"${MCU_VERSION_LIB}\")

if(NOT MCU_INCLUDE_PATH)
	if(NOT ENV{MCU_INCLUDE_PATH})
		message(FATAL_ERROR "******* $MCU_INCLUDE_PATH is not defined! ********")
	else()
		set(MCU_INCLUDE_PATH ENV{MCU_INCLUDE_PATH})
	endif()
endif()