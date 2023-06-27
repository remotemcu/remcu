
ifndef LLVM_ADIN_PATH
$(error "LLVM_ADIN_PATH variable not set!")
endif

CLANG			= clang
CLANGPP 		= clang++
AR 				= llvm-ar

BUILD_DIR 	= $(BUILD_DIRECTORY)

OPT_FLAGS += -adin -S

IR_FLAGS += -S -emit-llvm -DREMCU_LIB

IR_FLAGS += -I $(MCU_UTIL_PATH)/include_utils
IR_FLAGS += $(COMPILE_FLAGS)

CBE = echo

IR_SUFFIX = ll

OPT			= "$(LLVM_ADIN_PATH)/opt"
LD 			= "$(LLVM_ADIN_PATH)/llvm-link"

all: clean_build $(BUILD_DIR) $(OUTPUT)


.PHONY: clean_build
clean_build:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

print-%  : ; @echo $* = $($*)


LOCAL_CFLAGS = $(DEFS) $(INCLUDE_LOCAL) $(EXTRA_FLAGS)

IR_FILES += $(C_SRC:.c=.ll)
IR_FILES += $(CPP_SRC:.cpp=.ll)

$(OUTPUT): $(IR_FILES)
	$(LD) -v $(BUILD_DIR)/*.adin.ll -S -o $@.ll
	$(CLANG) $(COMPILE_FLAGS) -c $@.ll -o $@
	#$(AR) rcv $@.ar $(BUILD_DIR)/*.adin.o

REMCU_C: $(IR_FILES)
	@echo done


# compile
#------------------------------------------------------------------------------- 
%.ll: %.c
	$(CLANG) $(IR_FLAGS) $(LOCAL_CFLAGS) -c $< -o $(BUILD_DIR)/$(notdir $<).$(IR_SUFFIX)
	$(OPT) $(OPT_FLAGS) $(BUILD_DIR)/$(notdir $<).ll -o  $(BUILD_DIR)/$(notdir $<).adin.ll
# 	$(CBE) $(CBE_OPTIONS) $(BUILD_DIR)/$(notdir $<).adin.ll -o $(OUTPUT_DIR)/$(notdir $<).remcu.c
# 	$(CBE) $(CBE_OPTIONS) $(BUILD_DIR)/$(notdir $<).$(IR_SUFFIX) -o $(OUTPUT_MCU_DIR)/$(notdir $<).mcu.c

	#$(CLANG) $(COMPILE_FLAGS) -c $(BUILD_DIR)/$(notdir $<).adin.ll -o $(BUILD_DIR)/$(notdir $<).adin.o

%.ll: %.cpp
	$(CLANGPP) $(IR_FLAGS) $(LOCAL_CFLAGS) -c $< -o $(BUILD_DIR)/$(notdir $<).$(IR_SUFFIX)
	$(OPT) $(OPT_FLAGS) $(BUILD_DIR)/$(notdir $<).ll -o  $(BUILD_DIR)/$(notdir $<).adin.ll
