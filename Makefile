OS := $(shell uname)

ifneq (,$(findstring CYGWIN,$(OS)))
OS = CYGWIN
endif

ifeq ($(OS),CYGWIN)
CC			= "$(CLANG_PATH)clang++"
AR 			= "$(CLANG_PATH)llvm-ar"
OPT			= "$(OPT_PATH)opt"
else
CC			= clang++
AR 			= llvm-ar
endif

CFLAGS		= -c -std=c++14 -fno-rtti -pedantic-errors -Wall -Wextra -Werror
LDFLAGS		= -lstdc++

LAST_TAG = $(shell git describe --tags)
COMMIT_HASH = $(shell git rev-parse --short HEAD)
DEFINES += -D_GIT_HASH_=\"$(COMMIT_HASH)\" -D_GIT_TAG_=\"$(LAST_TAG)\" 

ifneq ($(DEBUG),)
CFLAGS += -g -D_DEBUG_BUILD_
else
CFLAGS += -O2
endif


ifeq ($(OS),Linux)
CFLAGS		+= -fPIC -fvisibility=hidden
LDFLAGS		+= -fPIC -fvisibility=hidden
else ifeq ($(OS),CYGWIN)
CFLAGS		+= -Xclang -flto-visibility-public-std -fno-exceptions 
LDFLAGS		+= -lwsock32  -Xclang -flto-visibility-public-std -fno-exceptions
DEFINES 	+= -D_CRT_SECURE_NO_WARNINGS -D_HAS_STATIC_RTTI=0
endif

SHARED_FLAGS = -shared

CFLAGS		+= $(DEFINES)

BUILD_DIR 	= build

ifeq ($(OS),CYGWIN)
NET_SOURCE = $(wildcard src/win/*.cpp)
else
NET_SOURCE = $(wildcard src/unix/*.cpp)
endif

SOURCES 	= $(wildcard src/*.cpp)

SOURCES 	+= $(NET_SOURCE)

OBJECTS		= $(SOURCES:.cpp=.o)

INCLUDES	= -I. -I include/ -I export/

TEST_SOURCE = test/test.cpp
TEST_OBJECTS= $(TEST_SOURCE:.cpp=.o)

TEST_IR_SOURCE = test/IrTest.cpp
TEST_IR_LL = $(BUILD_DIR)/$(notdir $(TEST_IR_SOURCE:.cpp=.ll))

LIB_NAME 	= libremcu

ifeq ($(OS),Linux)
STATIC_LIB 	= $(LIB_NAME).a
SHARED_LIB 	= $(LIB_NAME).so
else ifeq ($(OS),CYGWIN)
STATIC_LIB 	= $(LIB_NAME).lib
SHARED_LIB 	= $(LIB_NAME).dll
EXE_SUFFIX = .exe
endif

print-%  : ; @echo $* = $($*)


.PHONY: objects
objects: $(BUILD_DIR) $(OBJECTS)



.PHONY: static
static: $(BUILD_DIR) $(OBJECTS)
	$(AR) -rcs $(BUILD_DIR)/$(STATIC_LIB) $(BUILD_DIR)/*.o

.PHONY: shared
shared: $(BUILD_DIR) $(OBJECTS) $(TEST_IR_LL)
	$(CC) $(SHARED_FLAGS) $(LDFLAGS) $(TEST_IR_LL) $(BUILD_DIR)/*.o -o $(BUILD_DIR)/$(SHARED_LIB)


$(BUILD_DIR):
	mkdir -p $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $(BUILD_DIR)/$(notdir $@)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: test
test: $(BUILD_DIR) $(OBJECTS) $(TEST_OBJECTS) $(TEST_IR_LL)
	$(CC) $(LDFLAGS) $(BUILD_DIR)/*.o $(TEST_IR_LL) -o $(BUILD_DIR)/$@$(EXE_SUFFIX)


.PHONY: ir_pass
ir_pass: $(TEST_IR_SOURCE) $(TEST_IR_LL)


ifeq ($(OS),Linux)
$(TEST_IR_LL): $(TEST_IR_SOURCE)
	$(CC) -S -emit-llvm -Xclang -load -Xclang \
	../AddressInterception/build/src/libAddressInterceptorPassModule.so \
	$< -o $@
else
$(TEST_IR_LL): $(TEST_IR_SOURCE)
		$(CC) -S -emit-llvm $< -o $@ $(INCLUDES)
		$(OPT) -adin -S $@ -o $@
endif

.PHONY: strip
strip:
	strip -v --strip-all $(BUILD_DIR)/$(STATIC_LIB)