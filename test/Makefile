
include $(REMCU_PATH_MK)/remcu.mk


all: $(BUILD_DIR)/TEST.adin.o

$(BUILD_DIR)/TEST.adin.o: clean_build $(BUILD_DIR) IrTest.cpp
	$(CC) $(IR_FLAGS) IrTest.cpp -o $(BUILD_DIR)/IrTest.cpp.ll
	$(OPT) $(ADIN_FLAGS) $(BUILD_DIR)/IrTest.cpp.ll -o $(BUILD_DIR)/IrTest.cpp.adin.ll
	$(CC)  $(CFLAGS) -c $(BUILD_DIR)/IrTest.cpp.adin.ll -o $@


