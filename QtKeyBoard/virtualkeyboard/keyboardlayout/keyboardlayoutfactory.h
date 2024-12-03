// keyboardlayoutfactory.h
#ifndef LANGUAGELAYOUTFACTORY_H
#define LANGUAGELAYOUTFACTORY_H

#include "abstractlayout.h"

class KeyboardLayoutFactory
{
public:
    static AbstractLayout* CreateKeyboardLayout(const QString& type, QWidget* parent = nullptr);
};

#endif // LANGUAGELAYOUTFACTORY_H