
SOURCES += \
    ../src/qtnet.cpp \
    ../../src/logger.cpp \
    ../../src/assertion.cpp \
    ../../src/addressintercept.cpp \
    ../../src/OpenocdClient.cpp \
    ../../src/gdbclient.cpp        \
    ../../src/BaseClient.cpp

HEADERS += \
    ../../export/adin.h \
    ../../export/AddressInterceptPass.h \
    ../../include/client.h \
    ../../include/netwrapper.h \
    ../../include/assertion.h \
    ../../include/logger.h

INCLUDEPATH += ../../include/
INCLUDEPATH += ../../export/
INCLUDEPATH += ../../
