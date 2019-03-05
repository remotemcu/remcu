
if(NOT DEFINED MCU_LIB_CMAKE)
	message(FATAL_ERROR "******* $MCU_LIB_CMAKE is not defined! ********")
endif()

if(NOT DEFINED MCU_TYPE)
	message(FATAL_ERROR "******* $MCU_TYPE is not defined! ********")
endif()

if(NOT DEFINED COPY_SOURCE_FROM_LIB)
	message(FATAL_ERROR "******* $COPY_SOURCE_FROM_LIB is not defined! ********")
endif()

if(NOT DEFINED CONF_SOURCE)
	message(FATAL_ERROR "******* $CONF_SOURCE is not defined! ********")
endif()


set(REMCU_PATH_MK ${CMAKE_CURRENT_SOURCE_DIR}/target.mk)