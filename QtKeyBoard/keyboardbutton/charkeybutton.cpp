#include "charkeybutton.h"

#include <QDebug>

#include "inputmethodmgr.h"

CharKeyButton::CharKeyButton(QWidget* parent)
    :KeyBoardButton(parent)
{

}

void CharKeyButton::SlotKeyClicked()
{
    int keyCode = GetKey();
    KeyBoard::ClickKey(keyCode);
}

void CharKeyButton::SlotCapsSwitch(bool state)
{
  
}

void CharKeyButton::SlotShiftSwitch(bool state)
{
    QChar ch = text().front();
    if (state)
    {
        if (KeyBoard::ShiftMap.contains(ch))
        {
            setText(KeyBoard::ShiftMap[ch]);
        }
    }
    else
    {
        if (KeyBoard::ShiftMapRevert.contains(ch))
        {
            setText(KeyBoard::ShiftMapRevert[ch]);
        }
    }
}
