BUILD_DIR 	= $(BUILD_DIRECTORY)

ifndef LLVM_ADIN_PATH
$(error "LLVM_ADIN_PATH variable not set!")
endif

CLANG			= clang
CLANGPP 		= clang++
AR 			= llvm-ar
COMPILE_FLAGS 		= -fPIC --target=arm-linux-gnueabihf --gcc-toolchain=/home/ser/MOCD/raspberry/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/

#OPT			= "$(LLVM_ADIN_PATH)opt"
OPT = echo
LD 			= "$(LLVM_ADIN_PATH)llvm-link"

IR_FLAGS += -S -emit-llvm --target=arm-linux-gnueabihf
OPT_FLAGS = -adin -S

IR_FLAGS += -I $(MCU_UTIL_PATH)/include_export

IR_FLAGS += -Xclang -load -Xclang /home/ser/MOCD/AddressInterceptor/AddressInterceptorLib/build/../../AddressInterception/build/src/libAddressInterceptorPassModule.so

IR_SUFFIX = adin.ll

all: clean_build $(BUILD_DIR) $(OUTPUT)


.PHONY: clean_build
clean_build:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

print-%  : ; @echo $* = $($*)