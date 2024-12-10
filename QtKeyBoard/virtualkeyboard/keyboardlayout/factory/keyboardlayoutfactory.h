// keyboardlayoutfactory.h
#ifndef LANGUAGELAYOUTFACTORY_H
#define LANGUAGELAYOUTFACTORY_H

#include "abstractlayout.h"
#include "inputmethod.h"

class KeyboardLayoutFactory
{
public:
    static AbstractLayout* CreateKeyboardLayout(const KeyBoard::Language& language, QWidget* parent = nullptr);
};

#endif // LANGUAGELAYOUTFACTORY_H