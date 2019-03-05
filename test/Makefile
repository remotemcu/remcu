
include $(REMCU_PATH_MK)/remcu.mk


$(OUTPUT): IrTest.cpp
	$(CC) $(IR_FLAGS) -I../include -I. -I../export IrTest.cpp -o $(BUILD_DIR)/IrTest.cpp.ll
	$(OPT) $(ADIN_FLAGS) $(BUILD_DIR)/IrTest.cpp.ll -o $(BUILD_DIR)/IrTest.cpp.adin.ll
	$(CC)  $(CFLAGS) -c $(BUILD_DIR)/IrTest.cpp.adin.ll -o $@


