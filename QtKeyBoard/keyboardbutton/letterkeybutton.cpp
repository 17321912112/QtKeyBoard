#include "letterkeybutton.h"

#include <QDebug>

#include "inputmethodmgr.h"


LetterKeyButton::LetterKeyButton(QWidget* parent)
    :KeyBoardButton(parent)
{

}

void LetterKeyButton::SlotKeyClicked()
{
    int keyCode = GetKey();
    KeyBoard::ClickKey(keyCode);
}

void LetterKeyButton::SlotCapsSwitch(bool state)
{
    QChar ch = GetKey();
    if (state)
    {
        if (ch.isLower())
        {
            ch = ch.toUpper();
        }
    }
    else
    {
        if (ch.isUpper())
        {
            ch = ch.toLower();
        }
    }
    setText(ch);
}

void LetterKeyButton::SlotShiftSwitch(bool state)
{
    QChar ch = GetKey();
    if (state)
    {
        if (ch.isLower())
        {
            ch = ch.toUpper();
        }
    }
    else
    {
        if (ch.isUpper())
        {
            ch = ch.toLower();
        }
    }
    setText(ch);
}
