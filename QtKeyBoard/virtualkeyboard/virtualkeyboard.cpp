#include "virtualkeyboard.h"

#include <QApplication>
#include <QInputMethod>
#include <QScreen>
#include <QWindow>
#include <QDebug>

class VirtualKeyBoard::CPrivate
{
public:
    CPrivate(VirtualKeyBoard* parent)
        : mParent(parent)
    {

    }
    void InitUi();
    VirtualKeyBoard* mParent;
    languageLayout* mLayout;
};

void VirtualKeyBoard::CPrivate::InitUi()
{
    mLayout = new languageLayout();
}

VirtualKeyBoard *VirtualKeyBoard::mKeyBoard = nullptr;

VirtualKeyBoard::VirtualKeyBoard(QObject *parent)
    : QObject(parent)
    , md(new CPrivate(this))
{
    md->InitUi();
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

languageLayout* VirtualKeyBoard::GetLayout()
{
    return md->mLayout;
}


void VirtualKeyBoard::InstallKeyBoard(QApplication *app)
{
    QObject::connect(app->inputMethod(), &QInputMethod::cursorRectangleChanged,
                        this, [&](){
           QWindow *focusWindow = app->focusWindow();
           if (focusWindow && app->focusWidget() && !md->mLayout->isVisible()) {
               QRect rect = app->inputMethod()->cursorRectangle().toRect().translated(focusWindow->position()); // 获取光标位置
               QPoint pos = rect.bottomLeft() + QPoint(0, 5); //
               QScreen *screen = app->screenAt(pos); // 获取当前屏幕
               if (screen == Q_NULLPTR)
                   screen = app->primaryScreen();

               if (pos.x() + md->mLayout->width() > screen->geometry().width())
                   pos.setX(screen->geometry().width() - md->mLayout->width());
               if (pos.y() + md->mLayout->height() > screen->geometry().height())
                   pos.setY(screen->geometry().height() - md->mLayout->height());

                md->mLayout->move(pos);
                md->mLayout->show();
               
           }
       });
}