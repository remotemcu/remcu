BUILD_DIR 	= build

ifeq ($(OS),CYGWIN)
CC			= "$(CLANG_PATH)clang++"
AR 			= "$(CLANG_PATH)llvm-ar"
else
CC			= clang++
AR 			= llvm-ar
endif

OPT			= "$(LLVM_ADIN_PATH)opt"

IR_FLAGS = -S -emit-llvm
ADIN_FLAGS = -adin -S
CFLAGS = -fPIC


.PHONY: clean_build
clean_build:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

all: clean_build $(BUILD_DIR) $(OUTPUT)