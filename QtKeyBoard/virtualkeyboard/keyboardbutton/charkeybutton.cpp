#include "charkeybutton.h"

#include <QDebug>

#include "inputmethod.h"

CharKeyButton::CharKeyButton(QWidget* parent)
    :KeyBoardButton(parent)
    ,mLanguageState(true)
    ,mShiftState(false)
    ,mCapsLockState(false)
{

}

// void CharKeyButton::SlotKeyClicked()
// {
//     int keyCode = GetKey();
//     KeyBoard::ClickKey(keyCode);
// }

void CharKeyButton::CapsSwitch(bool state)
{
    mCapsLockState = state;
    RefreshText();
}

void CharKeyButton::ShiftSwitch(bool state)
{
    mShiftState = state;
    RefreshText();
}


void CharKeyButton::ResetKey()
{
    QString str = text();
    if (KeyBoard::EnglishCharMapRevert.contains(str))
    {
        setText(KeyBoard::EnglishCharMapRevert[str]);
    }
    else if (KeyBoard::ChineseCharMapRevert.contains(str))
    {
        setText(KeyBoard::ChineseCharMapRevert[str]);
    } 
    else if (KeyBoard::ChineseMapEnglish.contains(str))
    {
        setText(KeyBoard::ChineseMapEnglish[str]);
    }
    else if (KeyBoard::ChineseCharMapRevertWithoutShift.contains(str))
    {
        setText(KeyBoard::ChineseCharMapRevertWithoutShift[str]);
    }
}

void CharKeyButton::LanguageSwitch(bool state)
{
    mLanguageState = state;
    RefreshText();
}

void CharKeyButton::RefreshText()
{
    QString str = text();
    if (!mShiftState) // 不开SHIFT
    {
        if (mLanguageState || mCapsLockState)
        {
            ResetKey(); // 直接切换为英文原始字符键码
        }
        else if (!mLanguageState && !mCapsLockState)
        {
            ResetKey();
            if (KeyBoard::ChineseCharMapWithoutShift.contains(str))
            {
                setText(KeyBoard::ChineseCharMapWithoutShift[str]); // 显示中文字符
            }
        }
    } 
    else // 开SHIFT
    {
        if (!mLanguageState && !mCapsLockState) // 显示中文字符
        {
            ResetKey();
            if (KeyBoard::ChineseCharMap.contains(str))
            {
                setText(KeyBoard::ChineseCharMap[str]); 
            }
        }
        else
        {
            ResetKey();
            if (KeyBoard::EnglishCharMap.contains(str))
            {
                setText(KeyBoard::EnglishCharMap[str]);
            }
        }
    }
    
}
