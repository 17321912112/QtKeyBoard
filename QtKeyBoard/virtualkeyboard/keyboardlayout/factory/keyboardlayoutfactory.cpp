// languagelayoutfactory.cpp
#include "keyboardlayoutfactory.h"
#include "chineselayout.h"
#include <QDebug>

AbstractLayout* KeyboardLayoutFactory::CreateKeyboardLayout(const KeyBoard::Language& language, QWidget* parent)
{
    AbstractLayout* layout = nullptr;
    switch (language)
    {
    case KeyBoard::Language_Chinese:
        layout = new ChineseLayout(parent);
        break;
    default:
        break;
    }
    return layout;
}
