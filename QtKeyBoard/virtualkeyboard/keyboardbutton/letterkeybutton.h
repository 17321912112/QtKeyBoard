#ifndef LETTERKEYBUTTON_H
#define LETTERKEYBUTTON_H
#include "keyboardbutton.h"

class LetterKeyButton : public KeyBoardButton
{
public:
    LetterKeyButton(QWidget* parent = nullptr);

public slots:
    // void SlotKeyClicked() override; // 按键按下
public:
    void CapsSwitch(bool state) override; // 大小写切换
    void ShiftSwitch(bool state) override;// 字符切换
    void ResetKey() override; // 重置按键
};

#endif // LETTERKEYBUTTON_H
