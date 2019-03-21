
include $(TARGET_MK)

SRC = Ir.cpp

$(OUTPUT): $(SRC)
	$(CLANGPP) $(IR_FLAGS) -I. $(SRC) -o $(BUILD_DIR)/$(SRC).$(IR_SUFFIX)
	$(OPT) $(OPT_FLAGS) $(BUILD_DIR)/$(SRC).ll -o $(BUILD_DIR)/$(SRC).adin.ll
	$(CLANGPP)  $(COMPILE_FLAGS) -c $(BUILD_DIR)/$(SRC).adin.ll -o $@


