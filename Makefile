CC		= g++
CFLAGS		= -g -c -Wall
LDFLAGS		= 
SOURCES		= client.cpp tcpstream.cpp tcpconnector.cpp ocdcommand.cpp unixnet.cpp
INCLUDES	= -I.
OBJECTS		= $(SOURCES:.cpp=.o)
TARGET		= test

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -rf $(OBJECTS) $(TARGET)

