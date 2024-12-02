INCLUDEPATH +=  $$PWD

include($$PWD/inputmethod/inputmethod.pri)
include($$PWD/keyboardbutton/keyboardbutton.pri)
include($$PWD/languagelayout/languagelayout.pri)

SOURCES += \
    $$PWD/virtualkeyboard.cpp

HEADERS += \
    $$PWD/virtualkeyboard.h
