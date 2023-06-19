
ifeq ($(CMAKE_BUILD_TYPE),Debug)
IR_FLAGS = -D_DEBUG -D_MT -D_DLL 
endif

include $(MCU_UTIL_PATH)/common.mk