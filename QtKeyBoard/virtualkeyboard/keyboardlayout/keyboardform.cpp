#include "keyboardform.h"
#include "ui_virtualkeyboardform.h"
#include "abstractlayout.h"
#include "keyboardlayoutfactory.h"
#include "inputform.h"
#include "characterform.h"
#include <QStackedWidget>

class KeyBoardForm::CPrivate
{
public:
    CPrivate(KeyBoardForm* parent)
        : mParent(parent)
        , ui(new Ui::VirtualKeyBoardForm)
        , keyboard(nullptr)
        , characterform(nullptr)
        , input(nullptr)
    {
        ui->setupUi(parent);
    }

    void SetLanguage(KeyBoard::Language language);

    void InitUi();

    void InitConnect();

    KeyBoardForm *mParent;
    Ui::VirtualKeyBoardForm *ui;
    AbstractLayout *keyboard;
    CharacterForm *characterform;
    InputForm *input;
};

KeyBoardForm::KeyBoardForm(QWidget *parent)
    : QWidget(parent)
    , md(new CPrivate(this))
{
    // 不改变输入焦点
    this->setWindowFlags(Qt::FramelessWindowHint
                         | Qt::WindowStaysOnTopHint
                         | Qt::Tool);
#if QT_VERSION >= 0x050000
      this->setWindowFlags(this->windowFlags() | Qt::WindowDoesNotAcceptFocus);
#endif
    md->InitUi();
    md->InitConnect();
}

void KeyBoardForm::SetText(const QString &text)
{
    md->input->SetText(text);
}

void KeyBoardForm::SetLanguage(KeyBoard::Language language)
{
    md->SetLanguage(language);
}

void KeyBoardForm::showEvent(QShowEvent *event)
{
    // 使input显示在当前页面上层
    md->input->move(this->pos());
    md->ui->stackedWidget->setCurrentWidget(md->keyboard);
    md->input->show();
    QWidget::showEvent(event);
}

void KeyBoardForm::hideEvent(QHideEvent *event)
{
    md->input->hide();
    QWidget::hideEvent(event);
}

void KeyBoardForm::moveEvent(QMoveEvent *event)
{
    QWidget::moveEvent(event);
    md->input->move(this->pos());
}

void KeyBoardForm::CPrivate::SetLanguage(KeyBoard::Language language)
{
    // 换上新布局
    if (!keyboard)
    {
        keyboard->deleteLater();
    }
    keyboard = KeyboardLayoutFactory::CreateKeyboardLayout(language);
    keyboard->setFocusPolicy(Qt::NoFocus);
    ui->stackedWidget->addWidget(keyboard);
    connect(keyboard, &AbstractLayout::BackClicked, mParent, &KeyBoardForm::SlotSwitchLayout);
}

void KeyBoardForm::CPrivate::InitUi()
{
    input = new InputForm;
    // 不改变输入焦点
    input->setWindowFlags(Qt::FramelessWindowHint
                         | Qt::WindowStaysOnTopHint
                         | Qt::Tool);
    mParent->connect(input, &InputForm::SignalTextChanged, mParent, &KeyBoardForm::SignalInputChanged);
    characterform = new CharacterForm;
    ui->stackedWidget->addWidget(characterform);
}

void KeyBoardForm::CPrivate::InitConnect()
{
    mParent->connect(characterform, &CharacterForm::NewCharacterClicked, mParent, &KeyBoardForm::SlotNewCharacter);
    mParent->connect(characterform, &CharacterForm::BackClicked, mParent, &KeyBoardForm::SlotSwitchLayout);
}

bool KeyBoardForm::IsValidEdit(QLineEdit *edit)
{
    return md->input->IsValide(edit);
}

void KeyBoardForm::SetCursorPosition(int pos)
{
    md->input->SetCursorPosition(pos);
}

int KeyBoardForm::GetCursorPosition()
{
    return md->input->GetCursorPosition();
}

bool KeyBoardForm::InGeometry(const QPoint &point)
{
    QRect geomotry = geometry();
//    geomotry.setY(geomotry.y() - md->input->geometry().height());
    return geomotry.contains(point);
}

void KeyBoardForm::SlotNewCharacter(const QString &text)
{
    emit SignalCommited(text);
}

void KeyBoardForm::SlotSwitchLayout()
{
    if (md->ui->stackedWidget->currentWidget() == md->characterform)
    {
        md->ui->stackedWidget->setCurrentWidget(md->keyboard);
    }
    else
    {
        md->ui->stackedWidget->setCurrentWidget(md->characterform);
    }
}
