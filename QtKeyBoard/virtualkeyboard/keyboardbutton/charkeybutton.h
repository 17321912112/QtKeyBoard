#ifndef CHARKEYBUTTON_H
#define CHARKEYBUTTON_H

#include "keyboardbutton.h"

class CharKeyButton : public KeyBoardButton
{
public:
    CharKeyButton(QWidget* parent);

public slots:
    // void SlotKeyClicked() override; // 按键按下

public:
    void CapsSwitch(bool state) override; // 大小写切换
    void ShiftSwitch(bool state) override;// 字符切换
    void ResetKey() override;
    void LanguageSwitch(bool state); // 语言切换

private:
    void RefreshText();

private:
    bool mLanguageState; // 是否为英文状态
    bool mShiftState;    // Shift状态
    bool mCapsLockState; // 大写状态
};

#endif // CHARKEYBUTTON_H
