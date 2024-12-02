#include "keyboardbutton.h"

#include "inputmethodcontroller/inputmethodmgr.h"
#include <QDebug>

class KeyBoardButton::CPrivate
{
public:
    CPrivate(KeyBoardButton* parent)
        : mParent(parent)
        , mKey(KeyBoard::Key_Unknown)
    {

    }
    void InitUi();

    KeyBoardButton* mParent;
    KeyBoard::KeyType mKey;
};

void KeyBoardButton::CPrivate::InitUi()
{
    // 按键默认样式

}

KeyBoardButton::KeyBoardButton(QWidget* parent)
    :QPushButton(parent)
    ,md(new CPrivate(this))
{
    InitUI();
}


void KeyBoardButton::SetKey(int key)
{
    md->mKey = (KeyBoard::KeyType)key;
}

int KeyBoardButton::GetKey()
{
    return md->mKey;
}

void KeyBoardButton::InitUI()
{
    this->setStyleSheet("background-color: rgb(255, 255, 255);");
    // 设置字体占满按钮空间
    this->setFont(QFont("Microsoft YaHei", 15, QFont::Bold));
}

// void KeyBoardButton::InitConnect()
// {
//     connect(this, &QPushButton::clicked, this, &KeyBoardButton::SlotKeyClicked);
// }

void KeyBoardButton::SlotKeyClicked()
{
    int keyCode = GetKey();
    KeyBoard::ClickKey(keyCode);
    emit KeyReleased(keyCode);
    qDebug() << "KeyBoardButton::SlotKeyClicked" << keyCode;
}

void KeyBoardButton::CapsSwitch(bool state)
{
    
}

void KeyBoardButton::ShiftSwitch(bool state)
{
    
}

void KeyBoardButton::ResetKey()
{

}