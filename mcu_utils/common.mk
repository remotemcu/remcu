BUILD_DIR 	= $(BUILD_DIRECTORY)

OPT_FLAGS += -adin -S

IR_FLAGS += -S -emit-llvm -DREMCU_LIB

IR_FLAGS += -I $(MCU_UTIL_PATH)/include_utils

all: clean_build $(BUILD_DIR) $(OUTPUT)


.PHONY: clean_build
clean_build:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

print-%  : ; @echo $* = $($*)