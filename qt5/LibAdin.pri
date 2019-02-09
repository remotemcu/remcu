
SOURCES += \
    $$PWD/../src/logger.cpp \
    $$PWD/../src/assertion.cpp \
    $$PWD/../src/addressintercept.cpp \
    $$PWD/../src/OpenocdClient.cpp \
    $$PWD/../src/BaseClient.cpp \
    $$PWD/../src/GdbClient.cpp


HEADERS += \
    $$PWD/../export/adin.h \
    $$PWD/../include/AddressInterceptPass.h \
    $$PWD/../include/client.h \
    $$PWD/../include/netwrapper.h \
    $$PWD/../include/assertion.h \
    $$PWD/../include/logger.h \
    $$PWD/../include/defines.h

INCLUDEPATH += $$PWD/../include/
INCLUDEPATH += $$PWD/../export/
INCLUDEPATH += $$PWD/../
