
if(NOT DEFINED MCU_LIB_PATH)
	message(FATAL_ERROR "******* $MCU_LIB_PATH is not defined! ********")
endif()

if(NOT DEFINED MCU_TYPE)
	message(FATAL_ERROR "******* $MCU_TYPE is not defined! ********")
endif()

add_definitions(-D_MCU_TYPE_${MCU_TYPE})

if(NOT DEFINED COPY_OBJECT_FROM_LIB)
	message(FATAL_ERROR "******* $COPY_OBJECT_FROM_LIB is not defined! ********")
endif()