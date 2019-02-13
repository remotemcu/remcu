
SOURCES += \
    $$PWD/../src/logger.cpp \
    $$PWD/../src/assertion.cpp \
    $$PWD/../src/addressintercept.cpp \
    $$PWD/../src/OpenocdClient.cpp \
    $$PWD/../src/BaseClient.cpp \
    $$PWD/../src/GdbClient.cpp \
    $$PWD/../src/exportfunctions.cpp \
    $$PWD/../src/logofun.cpp


HEADERS += \
    $$PWD/../include/AddressInterceptPass.h \
    $$PWD/../include/client.h \
    $$PWD/../include/netwrapper.h \
    $$PWD/../include/assertion.h \
    $$PWD/../include/logger.h \
    $$PWD/../include/defines.h \
    $$PWD/../include/exports.h \
    $$PWD/../include/obusfaction.h \
    $$PWD/../include/logofun.h \
    $$PWD/../export/remcu.h


INCLUDEPATH += $$PWD/../include/
INCLUDEPATH += $$PWD/../export/
INCLUDEPATH += $$PWD/../

DISTFILES += \
    $$PWD/../src/unix/unixnet.cpp
