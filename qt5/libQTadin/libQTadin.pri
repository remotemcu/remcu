
SOURCES += \
    ../src/qtnet.cpp \
    ../../src/logger.cpp \
    ../../src/assertion.cpp \
    ../../src/addressintercept.cpp \
    ../../src/OpenocdClient.cpp \
    ../../src/BaseClient.cpp \
    $$PWD/../../src/GdbClient.cpp

HEADERS += \
    ../../export/adin.h \
    ../../export/AddressInterceptPass.h \
    ../../include/client.h \
    ../../include/netwrapper.h \
    ../../include/assertion.h \
    ../../include/logger.h \
    ../../include/defines.h

INCLUDEPATH += ../../include/
INCLUDEPATH += ../../export/
INCLUDEPATH += ../../
