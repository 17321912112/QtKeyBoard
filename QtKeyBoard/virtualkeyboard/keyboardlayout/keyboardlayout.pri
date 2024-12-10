INCLUDEPATH +=  $$PWD

SOURCES += \
    $$PWD/keyboardform.cpp

HEADERS += \
    $$PWD/keyboardform.h

FORMS += \
    $$PWD/virtualkeyboardform.ui

include($$PWD/layout/layout.pri)
include($$PWD/factory/factory.pri)
include($$PWD/inputform/inputform.pri)
