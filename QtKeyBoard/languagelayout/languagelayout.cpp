#include "languagelayout.h"
#include "ui_keyboardform.h"

#include "inputmethodmgr.h"
#include "keyboardbutton.h"

#include <windows.h>

#include <QMap>
#include <QDebug>
class languageLayout::CPrivate
{
public:
    CPrivate(languageLayout* parent)
        : mParent(parent)
        , ui(new Ui::VirtualKeyBoardForm)
        , mShiftPressed(false)
        , mCapsLockPressed(false)
    {
        ui->setupUi(parent);
    }

    void InitChinese();

    void Reset();

    languageLayout* mParent;
    Ui::VirtualKeyBoardForm *ui;
    QList<KeyBoardButton*> mKeyList;
    QMap<KeyBoard::KeyType, KeyBoardButton*> mKeyMap; // 按键映射
    bool mShiftPressed;
    bool mCapsLockPressed;

private slots:
    void Hide();
};

void languageLayout::CPrivate::Reset()
{
    if (mShiftPressed)
    {
        KeyBoard::ReleaseKey(KeyBoard::Key_LSHIFT);
        mShiftPressed = false;
        for (KeyBoardButton *btn : mKeyList)
        {
            btn->SlotShiftSwitch(false);
        }
    }
    if (mCapsLockPressed)
    {
        KeyBoard::ClickKey(KeyBoard::Key_CAPITAL);
        mCapsLockPressed = false;
        for (KeyBoardButton *btn : mKeyList)
        {
            btn->SlotShiftSwitch(false);
        }
    }

    ui->key_lshift->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");
    ui->key_rshift->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");
    ui->key_capsLock->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");
}

void languageLayout::CPrivate::InitChinese()
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
        {KeyBoard::Key_BACK, ui->key_backSpace}, {KeyBoard::Key_TAB, ui->key_tab}, {KeyBoard::Key_RETURN, ui->key_enter}, 
        // {KeyBoard::Key_CONVERT, ui->key_convert},  输入法转换
        {KeyBoard::Key_ESCAPE, ui->key_cancel},
        // {KeyBoard::Key_}
        {KeyBoard::Key_OEM_PLUS, ui->key_plus}, {KeyBoard::Key_OEM_MINUS, ui->key_minus},  {KeyBoard::Key_OEM_COMMA, ui->key_comma},
        {KeyBoard::Key_OEM_PERIOD, ui->key_dot_2}, {KeyBoard::Key_OEM_1, ui->key_semicolon},
        {KeyBoard::Key_OEM_2, ui->key_slash}, {KeyBoard::Key_OEM_3, ui->key_quotation},
        {KeyBoard::Key_OEM_4, ui->key_lbracket}, {KeyBoard::Key_OEM_5, ui->key_backslash}, {KeyBoard::Key_OEM_6, ui->key_rbracket},
        
    };

    // 切换按键处理
    connect(ui->key_switch, &QPushButton::clicked, [this](){
        KeyBoard::PressKey(KeyBoard::Key_LSHIFT);
        KeyBoard::PressKey(KeyBoard::Key_LCONTROL);
        KeyBoard::ReleaseKey(KeyBoard::Key_LSHIFT);
        KeyBoard::ReleaseKey(KeyBoard::Key_LCONTROL);
    });

    connect(ui->key_EN, &QPushButton::clicked, [this](){
        KeyBoard::ClickKey(KeyBoard::Key_LSHIFT);
    });
    
    // 普通按键处理
    for (auto iter = mKeyMap.begin(); iter != mKeyMap.end(); ++iter)
    {
        KeyBoardButton *btn = iter.value();
        btn->SetKey(iter.key());
        mKeyList << btn;
    }

    mKeyList << ui->key_lshift << ui->key_rshift << ui->key_capsLock;

    for (KeyBoardButton *btn : mKeyList)
    {
        if (btn->text() == "Shift")
        {
            connect(btn, &QPushButton::clicked, mParent, &languageLayout::SlotShiftSwitch);
        }
        else if (btn->text() == "Caps Lock")
        {
            connect(btn, &QPushButton::clicked, mParent, &languageLayout::SlotCapsSwitch);
        }
        else
        {
            connect(btn, &QPushButton::clicked, btn, &KeyBoardButton::SlotKeyClicked);
        }
    }

    // 特殊按键处理
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

void languageLayout::CPrivate::Hide()
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

languageLayout::languageLayout(QWidget *parent)
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

void languageLayout::showEvent(QShowEvent *event)
{
    md->Reset();
    QWidget::showEvent(event);
}

void languageLayout::SlotCapsSwitch(void)
{
    md->mCapsLockPressed = !md->mCapsLockPressed;
    KeyBoardButton *btn = md->ui->key_capsLock;
    if (md->mCapsLockPressed) // 变为按下状态
    {
        KeyBoard::ClickKey(KeyBoard::Key_CAPITAL);
        btn->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border: 2px solid rgb(0, 0, 0);}");
    }
    else
    {
        KeyBoard::ClickKey(KeyBoard::Key_CAPITAL);
        btn->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");
    }
    for (KeyBoardButton *btn : md->mKeyList)
    {
        btn->SlotCapsSwitch(md->mCapsLockPressed);
    }
}
void languageLayout::SlotShiftSwitch(void)
{
    md->mShiftPressed = !md->mShiftPressed;
    KeyBoardButton *btn = md->ui->key_lshift;
    if (md->mShiftPressed) // 变为按下状态
    {
        KeyBoard::PressKey(KeyBoard::Key_LSHIFT);
        btn->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border: 2px solid rgb(0, 0, 0);}");
    }
    else
    {
        KeyBoard::ReleaseKey(KeyBoard::Key_LSHIFT);
        btn->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");
    }
    for (KeyBoardButton *btn : md->mKeyList)
    {
        btn->SlotShiftSwitch(md->mShiftPressed);
    }
}