#CC		= g++
CC		= clang-6.0
CFLAGS		= -g -c -Wall -std=c++11
LDFLAGS		= -lstdc++
SOURCES		= test.cpp tcpstream.cpp tcpconnector.cpp ocdcommand.cpp unixnet.cpp addressintercept.cpp logger.cpp assertion.cpp
#llvm.cpp
INCLUDES	= -I.
OBJECTS		= $(SOURCES:.cpp=.o)
TARGET		= test

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) llvm.ll -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -rf $(OBJECTS) $(TARGET)

