// chineselayout.cpp
#include "chineselayout.h"
#include "ui_keyboardform.h"
#include "inputmethod.h"
#include "keyboardbutton.h"
#include "abstractlayout.h"
#include <QMap>
#include <QKeyEvent>
#include <QDebug>
#include <QThread>
class ChineseLayout::CPrivate
{
public:
    CPrivate(ChineseLayout* parent)
        : mParent(parent)
        , ui(new Ui::KeyBoardForm)
        , mShiftPressed(false)
        , mCapsLockPressed(false)
        , mLanguageState(true)
    {
        ui->setupUi(parent);
    }

    void InitChinese();

    void Reset();

    // 设置语言布局之后需调用Transfer来刷新ui
    void Transfer();

    ChineseLayout* mParent;
    Ui::KeyBoardForm *ui;
    QList<KeyBoardButton*> mKeyList;
    QMap<KeyBoard::KeyType, KeyBoardButton*> mKeyMap; // 按键映射
    bool mShiftPressed;
    bool mCapsLockPressed;
    bool mLanguageState; // 是否处于英文状态
};

void ChineseLayout::CPrivate::Reset()
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

    KeyBoard::LayoutMode curLanguage = KeyBoard::GetLanguageState();
    if (curLanguage == KeyBoard::Mode_Chinese)
    {
        KeyBoard::MicroSoftSwitch(); 
    }

    KeyBoard::SetSogouEnglish(); //切换为英文
    mLanguageState = true;

    for (KeyBoardButton* btn : mKeyList)
    {
        btn->ResetKey();
    }

    ui->key_lshift->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");
    ui->key_rshift->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");
    ui->key_capsLock->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");
    ui->key_EN->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");
}

void ChineseLayout::CPrivate::InitChinese()
{
    // 切换键盘
    connect(ui->key_back, &QPushButton::clicked, mParent, &AbstractLayout::BackClicked);
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
        // {KeyBoard::Key_LSHIFT, ui->key_lshift},
        // {KeyBoard::Key_RSHIFT, ui->key_rshift},
        {KeyBoard::Key_BACK, ui->key_backSpace},
        {KeyBoard::Key_TAB, ui->key_tab}, {KeyBoard::Key_RETURN, ui->key_enter},
        {KeyBoard::Key_ESCAPE, ui->key_cancel},
        {KeyBoard::Key_OEM_PLUS, ui->key_plus}, {KeyBoard::Key_OEM_MINUS, ui->key_minus},  {KeyBoard::Key_OEM_COMMA, ui->key_comma},
        {KeyBoard::Key_OEM_PERIOD, ui->key_decimal}, {KeyBoard::Key_OEM_1, ui->key_semicolon},
        {KeyBoard::Key_OEM_2, ui->key_divide}, {KeyBoard::Key_OEM_3, ui->key_backTick}, {KeyBoard::Key_OEM_4, ui->key_lbracket},
        {KeyBoard::Key_OEM_5, ui->key_backslash}, {KeyBoard::Key_OEM_6, ui->key_rbracket}, {KeyBoard::Key_OEM_7, ui->key_quote}
    };

    // // 删除键
    // connect(ui->key_backSpace, &QPushButton::pressed, [](){
    //     KeyBoard::ClickKey(KeyBoard::Key_BACK);
    // });

    // 全选
    connect(ui->key_selectAll, &QPushButton::clicked, [](){
        KeyBoard::PressKey(KeyBoard::Key_LCONTROL);
        KeyBoard::PressKey(KeyBoard::Key_A);
        KeyBoard::ReleaseKey(KeyBoard::Key_LCONTROL);
        KeyBoard::ReleaseKey(KeyBoard::Key_A);
    });

    // 切换输入法
    connect(ui->key_switch, &QPushButton::clicked, [this](){
        KeyBoard::PressKey(KeyBoard::Key_LSHIFT);
        KeyBoard::PressKey(KeyBoard::Key_LCONTROL);
        KeyBoard::ReleaseKey(KeyBoard::Key_LSHIFT);
        KeyBoard::ReleaseKey(KeyBoard::Key_LCONTROL);
        KeyBoard::SetSogouEnglish();
        mLanguageState = true;
        Transfer();
    });

    // 切换中英文
    connect(ui->key_EN, &QPushButton::clicked, mParent, &ChineseLayout::SlotSwitchLanguage);
    // 切换输入法的同时切换为中文
    connect(ui->key_switch, &QPushButton::clicked, mParent, &ChineseLayout::SlotSwitchLanguage);

    // 普通按键初始化
    for (auto iter = mKeyMap.begin(); iter != mKeyMap.end(); ++iter)
    {
        KeyBoardButton *btn = iter.value();
        btn->SetKey(iter.key());
        mKeyList << btn;
    }
    

    // 绑定shift和capslock按键
    mKeyList << ui->key_lshift << ui->key_rshift << ui->key_capsLock;

    // 按键事件绑定 Shift按键 capsLock按键事件
    for (KeyBoardButton *btn : mKeyList)
    {
        if (btn->text() == "Shift")
        {
            connect(btn, &QPushButton::clicked, mParent, &ChineseLayout::SlotShiftSwitch);
        }
        else if (btn->text() == "Caps Lock")
        {
            connect(btn, &QPushButton::clicked, mParent, &ChineseLayout::SlotCapsSwitch);
        }
        else
        {
            // 按键按下时通知界面
            connect(btn, &KeyBoardButton::SignalKeyReleased, mParent, &ChineseLayout::SlotKeyClicked);
        }
    }
}

void ChineseLayout::CPrivate::Transfer()
{
    // 通知按键根据语言状态刷新显示
    for (KeyBoardButton *btn : mKeyList)
    {
        CharKeyButton *charBtn = dynamic_cast<CharKeyButton*>(btn);
        if (charBtn)
        {
            charBtn->LanguageSwitch(mLanguageState);
        }
    }

    if (!mLanguageState)
    {
        ui->key_EN->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border: 2px solid rgb(0, 0, 0);}");
    }
    else
    {
        ui->key_EN->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");
    }
}

ChineseLayout::ChineseLayout(QWidget *parent)
    : AbstractLayout(parent)
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

void ChineseLayout::showEvent(QShowEvent *event)
{
    md->Reset();
    KeyBoard::GetLanguageState();
    AbstractLayout::showEvent(event);
}

// void ChineseLayout::SetCurrentLanguage(QString language)
// {
//     
// }

void ChineseLayout::SlotCapsSwitch(void)
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

void ChineseLayout::SlotShiftSwitch(void)
{
    if (md->mShiftPressed)
    {
        ShiftRelease();
        // 搜狗输入法在Shift释放时会切换布局语言，按下一次阻止其切换
        KeyBoard::ClickKey(KeyBoard::Key_LSHIFT);   
    }
    else
    {
        ShiftPress();
    }
}

void ChineseLayout::SlotKeyClicked(int keyCode)
{
    // 按键释放时，如果shift键按下，则松开shift
    if (md->mShiftPressed)
    {
        if ((KeyBoard::IsCharKey(keyCode) || KeyBoard::IsLetterKey(keyCode)))
        {
            ShiftRelease();
        }
    }
}

void ChineseLayout::ShiftPress()
{
    KeyBoard::PressKey(KeyBoard::Key_LSHIFT);
    md->mShiftPressed = true;
    for (KeyBoardButton *btn : md->mKeyList)
    {
        btn->ShiftSwitch(true);
    }
    md->ui->key_lshift->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border: 2px solid rgb(0, 0, 0);}");
    md->ui->key_rshift->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border: 2px solid rgb(0, 0, 0);}");
}

void ChineseLayout::CapsLockPress()
{
    md->mCapsLockPressed = !md->mCapsLockPressed;
    KeyBoard::ClickKey(KeyBoard::Key_CAPITAL);
    md->ui->key_capsLock->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);border: 2px solid rgb(0, 0, 0);}");
    for (KeyBoardButton *btn : md->mKeyList)
    {
        btn->CapsSwitch(md->mCapsLockPressed);
    }
}

void ChineseLayout::ShiftRelease()
{
    KeyBoard::ReleaseKey(KeyBoard::Key_LSHIFT);
    md->mShiftPressed = false;
    for (KeyBoardButton *btn : md->mKeyList)
    {
        btn->ShiftSwitch(false);
    }
    md->ui->key_lshift->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");
    md->ui->key_rshift->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");
}

void ChineseLayout::CapsLockRelease()
{
    md->mCapsLockPressed = !md->mCapsLockPressed;
    KeyBoard::ClickKey(KeyBoard::Key_CAPITAL);
    md->ui->key_capsLock->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");
    for (KeyBoardButton *btn : md->mKeyList)
    {
        btn->CapsSwitch(md->mCapsLockPressed);
    }
}

void ChineseLayout::SlotSwitchLanguage()
{
    KeyBoard::SogouSwitch();
    // md->mLanguageState = KeyBoard::GetLanguageState() == KeyBoard::Mode_English ? true : false;
    md->mLanguageState = KeyBoard::GetLanguage() == KeyBoard::Mode_English ? true : false;
    md->Transfer();
}

void ChineseLayout::hideEvent(QHideEvent *event)
{
    md->Reset();
    AbstractLayout::hideEvent(event);
}

void ChineseLayout::keyPressEvent(QKeyEvent *event)
{
    // 处理按键按下事件
    return QWidget::keyPressEvent(event);
}

void ChineseLayout::keyReleaseEvent(QKeyEvent *event)
{
    // 处理按键释放事件
    return QWidget::keyPressEvent(event);
}
