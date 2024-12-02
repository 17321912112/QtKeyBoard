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
}

void CharKeyButton::LanguageSwitch(bool state)
{
    mLanguageState = state;
    RefreshText();
}

void CharKeyButton::RefreshText()
{
    QString str = text();
    // qDebug() << "是否大写:" << mCapsLockState << "是否英文" << mLanguageState << "是否shift" << mShiftState;
    if (!mShiftState)
    {
        ResetKey(); // 直接切换为英文原始字符键
    } 
    else if (!mLanguageState && !mCapsLockState) // 小写且中文状态 显示中文字符
    {
        if (KeyBoard::ChineseCharMap.contains(str))
        {
            setText(KeyBoard::ChineseCharMap[str]); 
        }
        else if (KeyBoard::EnglishMapChinese.contains(str))
        {
            setText(KeyBoard::EnglishMapChinese[str]);
        }
    }
    else
    {
        if (KeyBoard::EnglishCharMap.contains(str))
        {
            setText(KeyBoard::EnglishCharMap[str]);
        }
        else if (KeyBoard::ChineseMapEnglish.contains(str))
        {
            setText(KeyBoard::ChineseMapEnglish[str]);
        }
    }
}
