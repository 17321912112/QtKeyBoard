INCLUDEPATH +=  $$PWD

include($$PWD/inputmethod/inputmethod.pri)
include($$PWD/keyboardbutton/keyboardbutton.pri)
include($$PWD/keyboardlayout/keyboardlayout.pri)

SOURCES += \
    $$PWD/virtualkeyboard.cpp

HEADERS += \
    $$PWD/virtualkeyboard.h
