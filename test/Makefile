
include $(REMCU_PATH_MK)/remcu.mk


$(OUTPUT): IrTest.cpp
	$(CLANG) $(IR_FLAGS) -I../include -I. -I../export IrTest.cpp -o $(BUILD_DIR)/IrTest.cpp.ll
	$(OPT) $(OPT_FLAGS) $(BUILD_DIR)/IrTest.cpp.ll -o $(BUILD_DIR)/IrTest.cpp.adin.ll
	$(CLANG)  $(COMPILE_FLAGS) -c $(BUILD_DIR)/IrTest.cpp.adin.ll -o $@


