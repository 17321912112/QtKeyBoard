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
};

#endif // CHARKEYBUTTON_H
