BUILD_DIR 	= build

OS_NAME := $(shell uname)

ifneq (,$(findstring CYGWIN,$(OS_NAME)))
OS_NAME = CYGWIN
endif

ifeq ($(OS_NAME),CYGWIN)
CC			= "$(CLANG_PATH)clang++"
AR 			= "$(CLANG_PATH)llvm-ar"
ifeq ($(CMAKE_BUILD_TYPE),Debug)
IR_FLAGS = -D_DEBUG -D_MT -D_DLL 
endif
else
CC			= clang++
AR 			= llvm-ar
CFLAGS 		= -fPIC
endif

OPT			= "$(LLVM_ADIN_PATH)opt"

IR_FLAGS += -S -emit-llvm
ADIN_FLAGS = -adin -S



.PHONY: clean_build
clean_build:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

all: clean_build $(BUILD_DIR) $(OUTPUT)