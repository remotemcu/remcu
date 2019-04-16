include $(TARGET_MK)


$(OUTPUT): IrTest.cpp
	$(CLANGPP) $(IR_FLAGS) -I../include IrTest.cpp -o $(BUILD_DIR)/IrTest.cpp.$(IR_SUFFIX)
	$(OPT) $(OPT_FLAGS) $(BUILD_DIR)/IrTest.cpp.ll -o $(BUILD_DIR)/IrTest.cpp.adin.ll
	$(CLANGPP)  $(COMPILE_FLAGS) -c $(BUILD_DIR)/IrTest.cpp.adin.ll -o $@


