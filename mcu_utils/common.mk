
ifndef LLVM_ADIN_PATH
$(error "LLVM_ADIN_PATH variable not set!")
endif

CLANG			= clang
CLANGPP 		= clang++
AR 				= llvm-ar

BUILD_DIR 	= $(BUILD_DIRECTORY)

OPT_FLAGS += -adin -S

IR_FLAGS += -S -emit-llvm -DREMCU_LIB

IR_FLAGS += -I $(MCU_UTIL_PATH)/include_utils

CBE = echo

IR_SUFFIX = ll

OPT			= "$(LLVM_ADIN_PATH)opt"
LD 			= "$(LLVM_ADIN_PATH)llvm-link"

all: clean_build $(BUILD_DIR) $(OUTPUT)


.PHONY: clean_build
clean_build:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

print-%  : ; @echo $* = $($*)