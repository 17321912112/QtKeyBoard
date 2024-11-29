#include "languagelayout.h"
#include "ui_keyboardform.h"

#include "inputmethodmgr.h"
#include "keyboardbutton.h"

#include <windows.h>

#include <QMap>
#include <QKeyEvent>
#include <QDebug>
class LanguageLayout::CPrivate
{
public:
    CPrivate(LanguageLayout* parent)
        : mParent(parent)
        , ui(new Ui::VirtualKeyBoardForm)
        , mShiftPressed(false)
        , mCapsLockPressed(false)
    {
        ui->setupUi(parent);
    }

    void InitChinese();

    void Reset(); // 键盘复位

    LanguageLayout* mParent;
    Ui::VirtualKeyBoardForm *ui;
    QList<KeyBoardButton*> mKeyList;
    QMap<KeyBoard::KeyType, KeyBoardButton*> mKeyMap; // 按键映射
    bool mShiftPressed;
    bool mCapsLockPressed;

private slots:
    void Hide();
};

void LanguageLayout::CPrivate::Reset()
{
    if (mShiftPressed)   // Shift状态判断
    {
        KeyBoard::ReleaseKey(KeyBoard::Key_LSHIFT);
        mShiftPressed = false;
    }
    mCapsLockPressed = KeyBoard::GetKeyOpenState(KeyBoard::Key_CAPITAL); // 获取键盘CapsLock状态
    if (mCapsLockPressed) // CapsLock状态判断
    {
        KeyBoard::ClickKey(KeyBoard::Key_CAPITAL);
        mCapsLockPressed = false;
    }

    for (KeyBoardButton* btn : mKeyList)
    {
        btn->ResetKey();
    }

    ui->key_lshift->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");
    ui->key_rshift->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");
    ui->key_capsLock->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");
}

void LanguageLayout::CPrivate::InitChinese()
{
    // 初始化按键映射
    mKeyMap = 
    {
        {KeyBoard::Key_1, ui->key_1}, {KeyBoard::Key_2, ui->key_2}, {KeyBoard::Key_3, ui->key_3},
        {KeyBoard::Key_4, ui->key_4}, {KeyBoard::Key_5, ui->key_5}, {KeyBoard::Key_6, ui->key_6},
        {KeyBoard::Key_7, ui->key_7}, {KeyBoard::Key_8, ui->key_8}, {KeyBoard::Key_9, ui->key_9},
        {KeyBoard::Key_0, ui->key_0}, {KeyBoard::Key_A, ui->key_a}, {KeyBoard::Key_B, ui->key_b},
        {KeyBoard::Key_C, ui->key_c}, {KeyBoard::Key_D, ui->key_d}, {KeyBoard::Key_E, ui->key_e},
        {KeyBoard::Key_F, ui->key_f}, {KeyBoard::Key_G, ui->key_g}, {KeyBoard::Key_H, ui->key_h},
        {KeyBoard::Key_I, ui->key_i}, {KeyBoard::Key_J, ui->key_j}, {KeyBoard::Key_K, ui->key_k},
        {KeyBoard::Key_L, ui->key_l}, {KeyBoard::Key_M, ui->key_m}, {KeyBoard::Key_N, ui->key_n},
        {KeyBoard::Key_O, ui->key_o}, {KeyBoard::Key_P, ui->key_p}, {KeyBoard::Key_Q, ui->key_q},
        {KeyBoard::Key_R, ui->key_r}, {KeyBoard::Key_S, ui->key_s}, {KeyBoard::Key_T, ui->key_t},
        {KeyBoard::Key_U, ui->key_u}, {KeyBoard::Key_V, ui->key_v}, {KeyBoard::Key_W, ui->key_w},
        {KeyBoard::Key_X, ui->key_x}, {KeyBoard::Key_Y, ui->key_y}, {KeyBoard::Key_Z, ui->key_z},

        {KeyBoard::Key_SPACE, ui->key_space},

        /*{KeyBoard::Key_LSHIFT, ui->key_lshift}, {KeyBoard::Key_RSHIFT, ui->key_rshift},{KeyBoard::Key_CAPITAL, ui->key_capsLock},*/ 
         {KeyBoard::Key_TAB, ui->key_tab}, {KeyBoard::Key_RETURN, ui->key_enter}, 
        // {KeyBoard::Key_CONVERT, ui->key_convert},  输入法转换 {KeyBoard::Key_BACK, ui->key_backSpace},
        {KeyBoard::Key_ESCAPE, ui->key_cancel},
        // {KeyBoard::Key_}
        {KeyBoard::Key_OEM_PLUS, ui->key_plus}, {KeyBoard::Key_OEM_MINUS, ui->key_minus},  {KeyBoard::Key_OEM_COMMA, ui->key_comma},
        {KeyBoard::Key_OEM_PERIOD, ui->key_decimal}, {KeyBoard::Key_OEM_1, ui->key_semicolon},
        {KeyBoard::Key_OEM_2, ui->key_divide}, {KeyBoard::Key_OEM_3, ui->key_backTick}, {KeyBoard::Key_OEM_4, ui->key_lbracket}, 
        {KeyBoard::Key_OEM_5, ui->key_backslash}, {KeyBoard::Key_OEM_6, ui->key_rbracket}, {KeyBoard::Key_OEM_7, ui->key_quote}
        
    };
    // 删除键
    connect(ui->key_backSpace, &QPushButton::pressed, [](){
        KeyBoard::ClickKey(KeyBoard::Key_BACK);
    });
    // 全选
    connect(ui->key_selectAll, &QPushButton::clicked, [](){
        KeyBoard::PressKey(KeyBoard::Key_LCONTROL);
        KeyBoard::PressKey(KeyBoard::Key_A);
        KeyBoard::ReleaseKey(KeyBoard::Key_LCONTROL);
        KeyBoard::ReleaseKey(KeyBoard::Key_A);
    });
    // 切换输入法
    connect(ui->key_switch, &QPushButton::clicked, [](){
        KeyBoard::PressKey(KeyBoard::Key_LSHIFT);
        KeyBoard::PressKey(KeyBoard::Key_LCONTROL);
        KeyBoard::ReleaseKey(KeyBoard::Key_LSHIFT);
        KeyBoard::ReleaseKey(KeyBoard::Key_LCONTROL);
    });

    // 切换中英文
    connect(ui->key_EN, &QPushButton::clicked, [](){
        KeyBoard::ClickKey(KeyBoard::Key_LSHIFT);
        KeyBoard::GetLanguageState();
    });
    
    // 普通按键处理
    for (auto iter = mKeyMap.begin(); iter != mKeyMap.end(); ++iter)
    {
        KeyBoardButton *btn = iter.value();
        btn->SetKey(iter.key());
        mKeyList << btn;
    }

    for (KeyBoardButton *btn : mKeyList)
    {
        connect(btn, &KeyBoardButton::KeyReleased, mParent, &LanguageLayout::SlotKeyClicked);
    }

    // 绑定shift和capslock按键
    mKeyList << ui->key_lshift << ui->key_rshift << ui->key_capsLock;

    // Shift按键 capsLock按键事件
    for (KeyBoardButton *btn : mKeyList)
    {
        if (btn->text() == "Shift")
        {
            connect(btn, &QPushButton::clicked, mParent, &LanguageLayout::SlotShiftSwitch);
        }
        else if (btn->text() == "Caps Lock")
        {
            connect(btn, &QPushButton::clicked, mParent, &LanguageLayout::SlotCapsSwitch);
        }
        else
        {
            connect(btn, &QPushButton::clicked, btn, &KeyBoardButton::SlotKeyClicked);
            connect(btn, &KeyBoardButton::KeyReleased, mParent, &LanguageLayout::SlotKeyClicked);
        }
    }

    // 隐藏
    connect(ui->hide, &QPushButton::clicked, [this](){
        if (!mParent->isVisible())
        {
            mParent->show();
        }
        else
        {
            mParent->hide();
        }
    });
}

void LanguageLayout::CPrivate::Hide()
{
    if (!mParent->isVisible())
    {
        mParent->show();
    }
    else
    {
        mParent->hide();
    }
}

LanguageLayout::LanguageLayout(QWidget *parent)
    : QWidget(parent)
    , md(new CPrivate(this))
{
    // 中文键盘初始化
    md->InitChinese();

    // 不改变输入焦点
    this->setWindowFlags(Qt::FramelessWindowHint
                             | Qt::WindowStaysOnTopHint
                             | Qt::Tool);
    #if QT_VERSION >= 0x050000
        this->setWindowFlags(this->windowFlags() | Qt::WindowDoesNotAcceptFocus);
    #endif

}

void LanguageLayout::showEvent(QShowEvent *event)
{
    md->Reset();
    QWidget::showEvent(event);
}

void LanguageLayout::SlotCapsSwitch(void)
{
    if (md->mCapsLockPressed)
    {
        CapsLockRelease();
    }
    else
    {
        CapsLockPress();
    }
}
void LanguageLayout::SlotShiftSwitch(void)
{
    if (md->mShiftPressed) 
    {
        ShiftRelease();
    }
    else
    {
        ShiftPress();
    }
}

void LanguageLayout::SlotKeyClicked(int keyCode)
{
    if (md->mShiftPressed && (KeyBoard::IsCharKey(keyCode) || KeyBoard::IsLetterKey(keyCode)) )
    {
        ShiftRelease();
    }
}

void LanguageLayout::ShiftPress()
{
    qDebug() << "ShiftPress";
    KeyBoard::PressKey(KeyBoard::Key_LSHIFT);
    md->mShiftPressed = true;
    for (KeyBoardButton *btn : md->mKeyList)
    {
        btn->ShiftSwitch(true);
    }
    md->ui->key_lshift->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border: 2px solid rgb(0, 0, 0);}");
    md->ui->key_rshift->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border: 2px solid rgb(0, 0, 0);}");
}

void LanguageLayout::CapsLockPress()
{
    md->mCapsLockPressed = !md->mCapsLockPressed;
    KeyBoard::ClickKey(KeyBoard::Key_CAPITAL);
    md->ui->key_capsLock->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border: 2px solid rgb(0, 0, 0);}");
    for (KeyBoardButton *btn : md->mKeyList)
    {
        btn->CapsSwitch(md->mCapsLockPressed);
    }
}

void LanguageLayout::ShiftRelease()
{
    qDebug() << "ShiftRelease";
    KeyBoard::ReleaseKey(KeyBoard::Key_LSHIFT);
    md->mShiftPressed = false;
    for (KeyBoardButton *btn : md->mKeyList)
    {
        btn->ShiftSwitch(false);
    }
    md->ui->key_lshift->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");
    md->ui->key_rshift->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");
}

void LanguageLayout::CapsLockRelease()
{
    md->mCapsLockPressed = !md->mCapsLockPressed;
    KeyBoard::ClickKey(KeyBoard::Key_CAPITAL);
    md->ui->key_capsLock->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");
    for (KeyBoardButton *btn : md->mKeyList)
    {
        btn->CapsSwitch(md->mCapsLockPressed);
    }
}

void LanguageLayout::hideEvent(QHideEvent *event)
{
    md->Reset();
    QWidget::hideEvent(event);
}


void LanguageLayout::keyPressEvent(QKeyEvent *event)
{
    
}

void LanguageLayout::keyReleaseEvent(QKeyEvent *event)
{

}