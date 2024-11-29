#ifndef KEYBOARDBUTTON_H
#define KEYBOARDBUTTON_H

#include <QPushButton>

class KeyBoardButton : public QPushButton
{
    Q_OBJECT
public:
    explicit KeyBoardButton(QWidget* parent = nullptr);
    void SetKey(int key);
    int GetKey();

signals:
    void KeyReleased(int key);
    
private:
    void InitUI(); // 设置按键的字符，样式
    void InitConnect();

public slots:
    void SlotKeyClicked(); // 按键按下

public:
    virtual void ResetKey();             // 按键复位
    virtual void CapsSwitch(bool state); // 大写开启、关闭
    virtual void ShiftSwitch(bool state);// 字符切换

private:
    class CPrivate;
    CPrivate *const md;
};

#endif // KEYBOARDBUTTON_H
