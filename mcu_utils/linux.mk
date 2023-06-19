
COMPILE_FLAGS 	= -fPIC

ifeq ($(CMAKE_BUILD_TYPE),Debug)
IR_FLAGS 			+= -g
COMPILE_FLAGS 		+= -g
endif

ifdef CROSS_MK_FILE
include $(CROSS_MK_FILE)
endif

include $(MCU_UTIL_PATH)/common.mk
