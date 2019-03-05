
#include $(ADIN_PATH)/adin.mk

BUILD_DIR 	= build



$(BUILD_DIR)/TEST.adin.o: clean $(BUILD_DIR) IrTest.cpp
	clang -S -emit-llvm -I../include -I. -I../export IrTest.cpp -o $(BUILD_DIR)/IrTest.cpp.ll
	$(LLVM_ADIN_PATH)/opt -adin -S $(BUILD_DIR)/IrTest.cpp.ll -o $(BUILD_DIR)/IrTest.cpp.adin.ll
	clang -c -fPIC $(BUILD_DIR)/IrTest.cpp.adin.ll -o $@


$(BUILD_DIR):
	mkdir -p $@


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)