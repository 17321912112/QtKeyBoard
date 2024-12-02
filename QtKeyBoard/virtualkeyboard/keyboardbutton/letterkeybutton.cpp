#include "letterkeybutton.h"

#include <QDebug>

#include "inputmethod.h"


LetterKeyButton::LetterKeyButton(QWidget* parent)
    :KeyBoardButton(parent)
{

}

// void LetterKeyButton::SlotKeyClicked()
// {
//     int keyCode = GetKey();
//     KeyBoard::ClickKey(keyCode);
// }

void LetterKeyButton::CapsSwitch(bool state)
{
    Q_UNUSED(state);
    QChar ch = text().front();
    if (ch.isUpper())
    {
        ch = ch.toLower();
    }
    else if (ch.isLower())
    {
        ch = ch.toUpper();
    }
    setText(ch);
}

void LetterKeyButton::ShiftSwitch(bool state)
{
    Q_UNUSED(state);
    QChar ch = text().front();
    if (ch.isUpper())
    {
        ch = ch.toLower();
    }
    else if (ch.isLower())
    {
        ch = ch.toUpper();
    }
    setText(ch);
}

void LetterKeyButton::ResetKey()
{
    QChar ch = GetKey();
    setText(ch.toLower());
}