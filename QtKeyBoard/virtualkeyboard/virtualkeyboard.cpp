#include "virtualkeyboard.h"

#include <QApplication>
#include <QInputMethod>
#include <QScreen>
#include <QWindow>
#include <QDebug>

// class VirtualKeyBoard::CPrivate
// {
// public:
//     CPrivate(VirtualKeyBoard* parent)
//         : mParent(parent)
//     {

//     }
//     void InitUi();
//     VirtualKeyBoard* mParent;
//     LanguageLayout* mLayout;
// };

// void VirtualKeyBoard::CPrivate::InitUi()
// {
//     mLayout = new LanguageLayout();
// }

VirtualKeyBoard *VirtualKeyBoard::mKeyBoard = nullptr;

VirtualKeyBoard::VirtualKeyBoard(QObject *parent)
    : QObject(parent)
    // , md(new CPrivate(this))
{
    mLayout = KeyboardLayoutFactory::CreateKeyboardLayout("Chinese");
    if (!mLayout)
    {
        qWarning() << "Failed to create default keyboard layout.";
    }
}

VirtualKeyBoard *VirtualKeyBoard::GetInstance()
{
    if (!mKeyBoard) 
    {
        if (!mKeyBoard) 
        {
            mKeyBoard = new VirtualKeyBoard();
        }
    }
    return mKeyBoard;
}

AbstractLayout* VirtualKeyBoard::GetLayout()
{
    return mLayout;
}

void VirtualKeyBoard::SetLanguage(KeyBoard::Language language)
{
    // 根据语言类型创建新的键盘布局
    QString languageType;
    switch (language) {
    case KeyBoard::Language_Chinese:
        languageType = "Chinese";
        break;
    default:
        qWarning() << "Unsupported language type:" << language;
        return;
    }

    // 删除旧的布局
    delete mLayout;
    mLayout = nullptr;

    // 创建新的布局
    mLayout = KeyboardLayoutFactory::CreateKeyboardLayout(languageType);
    if (!mLayout) {
        qWarning() << "Failed to create keyboard layout for language:" << language;
    }
}


void VirtualKeyBoard::InstallKeyBoard(QApplication *app)
{
    QObject::connect(app->inputMethod(), &QInputMethod::cursorRectangleChanged,
                        this, [&](){
           QWindow *focusWindow = app->focusWindow();
           if (focusWindow && app->focusWidget() && !mLayout->isVisible()) {
               QRect rect = app->inputMethod()->cursorRectangle().toRect().translated(focusWindow->position()); // 获取光标位置
               QPoint pos = rect.bottomLeft() + QPoint(0, 10); //
               QScreen *screen = app->screenAt(pos); // 获取当前屏幕
               if (screen == Q_NULLPTR)
                   screen = app->primaryScreen();

               if (pos.x() + mLayout->width() > screen->geometry().width())
                   pos.setX(screen->geometry().width() - mLayout->width());
               if (pos.y() + mLayout->height() > screen->geometry().height())
                   pos.setY(screen->geometry().height() - mLayout->height());

                mLayout->move(pos);
                mLayout->show();
               
           }
       });
}
