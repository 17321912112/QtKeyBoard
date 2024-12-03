// languagelayoutfactory.cpp
#include "keyboardlayoutfactory.h"
#include "chineselayout.h"
#include <QDebug>

AbstractLayout* KeyboardLayoutFactory::CreateKeyboardLayout(const QString& type, QWidget* parent)
{
    if (type == "Chinese")
    {
        return new ChineseLayout(parent);
    }
    else
    {
        qWarning() << "Unknown layout type:" << type;
        return nullptr;
    }
}
