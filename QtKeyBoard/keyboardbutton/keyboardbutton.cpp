#include "keyboardbutton.h"

#include "inputmethodcontroller/inputmethodmgr.h"

class KeyBoardButton::CPrivate
{
public:
    CPrivate(KeyBoardButton* parent);
    void InitUi();

    KeyBoardButton* mParent;
    KeyBoard::KeyType mKey;
    bool mIsPressed;
};
KeyBoardButton::CPrivate::CPrivate(KeyBoardButton* parent)
    :mParent(parent)
{

}

void KeyBoardButton::CPrivate::InitUi()
{
    // 按键默认样式

}

KeyBoardButton::KeyBoardButton(QWidget* parent)
    :QPushButton(parent)
    ,md(new CPrivate(this))
{

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
    // 按下时候的样式
    connect(this, &QPushButton::pressed, [this]() {
        md->mIsPressed = true;
        emit KeyPressed(md->mKey);
        // 颜色加深
        this->setStyleSheet("background-color: rgb(255, 255, 255);");
    });

    connect(this, &QPushButton::released, [this]() {
        md->mIsPressed = false;
        emit KeyReleased(md->mKey);
        this->setStyleSheet("background-color: rgb(255, 255, 255);");
    });
}
