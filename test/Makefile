
include $(MCU_MAKEFILE_PATH)


$(OUTPUT): IrTest.cpp
	$(CLANGPP) $(IR_FLAGS) -I../include -I. -I../export IrTest.cpp -o $(BUILD_DIR)/IrTest.cpp.ll
	$(OPT) $(OPT_FLAGS) $(BUILD_DIR)/IrTest.cpp.ll -o $(BUILD_DIR)/IrTest.cpp.adin.ll
	$(CLANGPP)  $(COMPILE_FLAGS) -c $(BUILD_DIR)/IrTest.cpp.adin.ll -o $@


