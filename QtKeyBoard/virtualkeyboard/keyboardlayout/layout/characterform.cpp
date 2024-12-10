#include "characterform.h"
#include "ui_characterform.h"

#include <QButtonGroup>
#include "inputmethod.h"

CharacterForm::CharacterForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CharacterForm)
{
    ui->setupUi(this);

    // 不改变输入焦点
    this->setWindowFlags(Qt::FramelessWindowHint
                         | Qt::WindowStaysOnTopHint
                         | Qt::Tool);
#if QT_VERSION >= 0x050000
    this->setWindowFlags(this->windowFlags() | Qt::WindowDoesNotAcceptFocus);
#endif

    Init();
}

CharacterForm::~CharacterForm()
{
    delete ui;
}

void CharacterForm::Init()
{

    for (auto *button : ui->buttonGroup->buttons())
    {
        QPushButton *btn = qobject_cast<QPushButton*>(button);
        if (!btn) continue;
        btn->setStyleSheet("background-color: rgb(255, 255, 255);");
        btn->setFont(QFont("Microsoft YaHei", 15, QFont::Bold));
        if (btn == ui->key_back)
        {
            connect(btn, &QPushButton::clicked, this, [=](){
                emit BackClicked();
            });
        }
        else if (btn == ui->key_backSpace)
        {
            connect(btn, &QPushButton::clicked, this, [=](){
                KeyBoard::ClickKey(VK_BACK);
            });

        }
        else
        {
            // 按钮按下时，向父窗口发送信号
            connect(btn, &QPushButton::clicked, this, [=](){
                emit NewCharacterClicked(btn->text());
            });
        }
    }
}
