
if(NOT DEFINED MCU_LIB_CMAKE)
	message(FATAL_ERROR "******* $MCU_LIB_CMAKE is not defined! ********")
endif()

if(NOT DEFINED MCU_TYPE)
	message(FATAL_ERROR "******* $MCU_TYPE is not defined! ********")
endif()

if(NOT DEFINED CONF_SOURCE)
	message(FATAL_ERROR "******* $CONF_SOURCE is not defined! ********")
endif()