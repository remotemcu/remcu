
OS_NAME := $(shell uname)

ifneq (,$(findstring CYGWIN,$(OS_NAME)))
OS_NAME = CYGWIN
endif

ifndef LLVM_ADIN_PATH
$(error "LLVM_ADIN_PATH variable not set!")
endif


ifeq ($(OS_NAME),CYGWIN)
CLANG			= "$(CLANG_PATH)clang"
CLANGPP 		= "$(CLANG_PATH)clang++"
AR 			= "$(LLVM_ADIN_PATH)llvm-ar"
ifeq ($(CMAKE_BUILD_TYPE),Debug)
IR_FLAGS = -D_DEBUG -D_MT -D_DLL 
endif
else
CLANG			= clang
CLANGPP 		= clang++
AR 			= llvm-ar
COMPILE_FLAGS 		= -fPIC
ifeq ($(CMAKE_BUILD_TYPE),Debug)
IR_FLAGS 			+= -g
COMPILE_FLAGS 		+= -g
endif
endif

OPT			= "$(LLVM_ADIN_PATH)opt"
LD 			= "$(LLVM_ADIN_PATH)llvm-link"

IR_SUFFIX = ll

include $(MCU_UTIL_PATH)/common.mk
