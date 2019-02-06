OS := $(shell uname)

CC			= clang++
AR 			= llvm-ar
CFLAGS		= -g -c -std=c++11 -fno-rtti -pedantic-errors -Wall -Wextra -Werror
LDFLAGS		= -lstdc++

ifeq ($(OS),Linux)
CFLAGS		+= -fPIC
LDFLAGS		+= -fPIC
SHARED_FLAGS = -shared
endif

BUILD_DIR 	= build


UNIX_SOURCE = $(wildcard src/unix/*.cpp)

SOURCES 	= $(wildcard src/*.cpp)

SOURCES 	+= $(UNIX_SOURCE)

OBJECTS		= $(SOURCES:.cpp=.o)

INCLUDES	= -I. -I include/ -I export/

TEST_SOURCE = test/test.cpp
TEST_OBJECTS= $(TEST_SOURCE:.cpp=.o)

TEST_IR_SOURCE = test/IrTest.cpp
TEST_IR_LL = $(TEST_IR_SOURCE:.cpp=.ll)

LIB_NAME 	= libadin

ifeq ($(OS),Linux)
STATIC_LIB 	= $(LIB_NAME).a
SHARED_LIB 	= $(LIB_NAME).so
endif

print-%  : ; @echo $* = $($*)


.PHONY: objects
objects: $(BUILD_DIR) $(OBJECTS)


.PHONY: static
static: $(BUILD_DIR) $(OBJECTS)
	$(AR) -rcs $(BUILD_DIR)/$(STATIC_LIB) $(BUILD_DIR)/*.o

.PHONY: shared
shared: $(BUILD_DIR) $(OBJECTS)
	$(CC) $(SHARED_FLAGS) $(LDFLAGS) $(BUILD_DIR)/*.o -o $(BUILD_DIR)/$(SHARED_LIB)


$(BUILD_DIR):
	mkdir -p $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $(BUILD_DIR)/$(notdir $@)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TEST_IR_LL)

.PHONY: test
test: $(BUILD_DIR) $(OBJECTS) $(TEST_OBJECTS) $(TEST_IR_LL)
	$(CC) $(LDFLAGS) $(BUILD_DIR)/*.o $(TEST_IR_LL) -o $(BUILD_DIR)/$@


.PHONY: ir_pass
ir_pass: $(TEST_IR_SOURCE) $(TEST_IR_LL)

$(TEST_IR_LL): $(TEST_IR_SOURCE)
	$(CC) -S -emit-llvm -Xclang -load -Xclang \
	../AddressInterceptorPass/build/AddressInterceptPass/libAddressInterceptPass.so \
	$< -o $@
