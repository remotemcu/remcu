
ifndef LLVM_ADIN_PATH
$(warning "LLVM_ADIN_PATH variable not set!")
endif

CLANG			= clang
CLANGPP 		= clang++
AR 			= llvm-ar
COMPILE_FLAGS 		= -fPIC --target=arm-linux-gnueabihf --gcc-toolchain=/home/ser/MOCD/raspberry/tools/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf/bin/

#OPT			= "$(LLVM_ADIN_PATH)opt"
OPT = echo
LD 			= "$(LLVM_ADIN_PATH)llvm-link"

IR_FLAGS += --target=arm-linux-gnueabihf

IR_FLAGS += -Xclang -load -Xclang /home/ser/MOCD/AddressInterceptor/AddressInterceptorLib/build/../../AddressInterception/build/src/libAddressInterceptorPassModule.so

IR_SUFFIX = adin.ll

include $(MCU_UTIL_PATH)/common.mk
