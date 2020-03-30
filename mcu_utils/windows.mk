
CLANG			= "$(CLANG_PATH)clang"
CLANGPP 		= "$(CLANG_PATH)clang++"
AR 			= "$(LLVM_ADIN_PATH)llvm-ar"

ifeq ($(CMAKE_BUILD_TYPE),Debug)
IR_FLAGS = -D_DEBUG -D_MT -D_DLL 
endif

include $(MCU_UTIL_PATH)/common.mk