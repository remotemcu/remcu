CC			= clang
AR 			= llvm-ar
CFLAGS		= -g -c -std=c++11 -fno-rtti -pedantic-errors -Wall -Wextra -Werror
#-fPIC
LDFLAGS		= -lstdc++

BUILD_DIR 	= build


UNIX_SOURCE = $(wildcard src/unix/*.cpp)

SOURCES 	= $(wildcard src/*.cpp)

SOURCES 	+= $(UNIX_SOURCE)

OBJECTS		= $(SOURCES:.cpp=.o)

INCLUDES	= -I. -I include/ -I export/

TEST		= test
TEST_SOURCE = test/test.cpp
TEST_OBJECTS= $(TEST_SOURCE:.cpp=.o)

TEST_IR_SOURCE = test/IrTest.cpp
TEST_IR_LL = $(TEST_IR_SOURCE:.cpp=.ll)

LIB_NAME 	= adin

STATIC_LIB 	= $(LIB_NAME).a

SHARED_LIB 	= $(LIB_NAME).so

.PHONY: lib
lib: $(SOURCES) $(STATIC_LIB)


$(STATIC_LIB): $(OBJECTS) $(BUILD_DIR)
	$(AR) -rcs $(BUILD_DIR)/$@ $(OBJECTS)


.PHONY: shared
shared: $(SOURCES) $(SHARED_LIB)

$(SHARED_LIB): $(OBJECTS) $(BUILD_DIR)
	$(CC) -shared $(LDFLAGS) $(OBJECTS) -o $(BUILD_DIR)/$@


$(BUILD_DIR):
	@mkdir -p build/

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJECTS) $(TARGET) $(TEST_OBJECTS) $(BUILD_DIR) $(TEST_IR_LL)

.PHONY: test
test: $(SOURCES) $(TEST_SOURCE) $(TEST)

$(TEST): $(OBJECTS) $(TEST_OBJECTS) $(TEST_IR_LL) $(BUILD_DIR)
	$(CC) $(LDFLAGS) $(OBJECTS) $(TEST_OBJECTS) $(TEST_IR_LL) -o $(BUILD_DIR)/$@


.PHONY: ir-test
ir-test: $(TEST_IR_SOURCE) $(TEST_IR_LL) $(BUILD_DIR)

$(TEST_IR_LL): $(TEST_IR_SOURCE)
	$(CC) -S -emit-llvm -Xclang -load -Xclang \
	../AddressInterceptorPass/build/AddressInterceptPass/libAddressInterceptPass.so \
	$< -o $@
