#include "keyboardbutton.h"

#include "inputmethodcontroller/inputmethodmgr.h"


class KeyBoardButton::CPrivate
{
public:
    CPrivate(KeyBoardButton* parent)
        : mParent(parent)
    {

    }
    void InitUi();

    KeyBoardButton* mParent;
    KeyBoard::KeyType mKey;
    bool mIsPressed;
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
    InitConnect();
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
    
}

void KeyBoardButton::InitConnect()
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

    connect(this, &QPushButton::clicked, [this]() {
        emit KeyClicked(md->mKey);
    });
    
}

void KeyBoardButton::CapsSwitch()
{
    QChar ch = QChar(this->text().front());
    if (ch.isLower())
    {
        this->setText(ch.toUpper());
    } else if (ch.isUpper())
    {
        this->setText(ch.toLower());
    } else {

    }
}

void KeyBoardButton::ShiftSwitch()
{
    QChar ch = QChar(this->text().front());
    if (ch.isLower())
    {
        this->setText(ch.toUpper());
    } else if (ch.isUpper())
    {
        this->setText(ch.toLower());
    } else {

    }
}
