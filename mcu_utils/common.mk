BUILD_DIR 	= $(BUILD_DIRECTORY)

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
endif

OPT			= "$(LLVM_ADIN_PATH)opt"
LD 			= "$(LLVM_ADIN_PATH)llvm-link"

IR_FLAGS += -S -emit-llvm -DREMCU_LIB
OPT_FLAGS = -adin -S

IR_FLAGS += -I $(MCU_UTIL_PATH)/include_utils

IR_SUFFIX = ll

all: clean_build $(BUILD_DIR) $(OUTPUT)


.PHONY: clean_build
clean_build:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

print-%  : ; @echo $* = $($*)