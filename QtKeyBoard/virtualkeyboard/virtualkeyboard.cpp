#include "virtualkeyboard.h"

#include <QApplication>
#include <QInputMethod>
#include <QScreen>
#include <QWindow>
#include <QDebug>
#include <QLineEdit>
#include <QTextEdit>
#include <QPlainTextEdit>

MouseEventFilter::MouseEventFilter(QObject *parent)
    : QObject(parent), m_keyboard(nullptr)
{
}

void MouseEventFilter::setKeyboard(QWidget *keyboard)
{
    m_keyboard = keyboard;
}

bool MouseEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) 
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (m_keyboard && !m_keyboard->geometry().contains(mouseEvent->globalPos())) 
        {
            m_keyboard->hide();
        }
    }
    return QObject::eventFilter(obj, event);
}


VirtualKeyBoard *VirtualKeyBoard::mKeyBoard = nullptr;

VirtualKeyBoard::VirtualKeyBoard(QObject *parent)
    : QObject(parent)
    , m_eventFilter(new MouseEventFilter(this))
    // , md(new CPrivate(this))
{
    mKeyboardform = new KeyBoardForm();
    LoadDefaultLayout();
    if (!mKeyboardform)
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

KeyBoardForm* VirtualKeyBoard::GetKeyBoardForm()
{
    return mKeyboardform;
}

void VirtualKeyBoard::SetLanguage(KeyBoard::Language language)
{
    // 设置键盘语言
    mKeyboardform->SetLanguage(language);
    if (!mKeyboardform)
    {
        qWarning() << "Failed to create keyboard layout for language:" << language;
    }
}


void VirtualKeyBoard::InstallKeyBoard(QApplication *app)
{
    InitKeyboardWidget(); // 安装事件过滤器

    // 计算位置 呼出键盘
    QObject::connect(app->inputMethod(), &QInputMethod::cursorRectangleChanged, this, [&](){
           QWindow *focusWindow = app->focusWindow();
        //    mWindow = app->focusWidget();
           if (focusWindow && app->focusWidget() && !mKeyboardform->isVisible())
           {
               SyncText(app->focusWidget());
               QRect rect = app->inputMethod()->cursorRectangle().toRect().translated(focusWindow->position()); // 获取光标位置
               QPoint pos = rect.bottomLeft() + QPoint(0, 30); //
               QScreen *screen = app->screenAt(pos); // 获取当前屏幕
               if (screen == Q_NULLPTR)
                   screen = app->primaryScreen();

               if (pos.x() + mKeyboardform->width() > screen->geometry().width())
               {
                   pos.setX(screen->geometry().width() - mKeyboardform->width());
               }
               if (pos.y() + mKeyboardform->height() > screen->geometry().height())
               {
                   pos.setY(screen->geometry().height() - mKeyboardform->height());
               }

                mKeyboardform->move(pos);
                mKeyboardform->show();
                // 移动主窗口以避免被软键盘遮挡

                // 计算软键盘与焦点窗口之间的距离
                // int x = pos.x() - rect.bottomLeft().x();// > 0 ? rect.x() - pos.x() : rect.x() - pos.x();
                // int y = pos.y() - rect.bottomLeft().y();// > 0 ? rect.y() - pos.y() : rect.y() - pos.y();
                // if (focusWindow)
                // {
                //     int newX = rect.x(), newY = newY = rect.y();
                //     if (x < 0)
                //     {
                //         newX = focusWindow->x() + x;
                //     }
                //     if (y < 0)
                //     {
                //         newY = focusWindow->y() + y;
                //     }
                //     if (x < 0 || y < 0)
                //     focusWindow->setPosition(newX, newY);
                // }
           }
       });
    // 字符显示
    QObject::connect(mKeyboardform, &KeyBoardForm::SignalCommited, [=](const QString &text){
        QInputMethodEvent *event = new QInputMethodEvent;
        event->setCommitString(text);
        QWidget *focusWindow = app ? app->focusWidget() : Q_NULLPTR;
        if (focusWindow)
        {
            app->postEvent(focusWindow, event);
        }
    });

    // 安装全局事件过滤器 实现在点击其他位置时隐藏键盘
    app->installEventFilter(m_eventFilter);
}

void VirtualKeyBoard::InitKeyboardWidget()
{
    if (mKeyboardform)
    {
        m_eventFilter->setKeyboard(mKeyboardform);
    }
}

void VirtualKeyBoard::LoadDefaultLayout()
{
    KeyBoard::Language language = KeyBoard::GetDefaultLanguage();
    mKeyboardform->SetLanguage(language);
}

void VirtualKeyBoard::SyncText(QWidget *focusWidget)
{
    // 清除之前的连接
    disconnect(mTextChangeConnection);
    disconnect(mTextChangeSignal);

    // 根据部件类型连接相应的信号
    if (QLineEdit *lineEdit = qobject_cast<QLineEdit*>(focusWidget))
    {
        if (mKeyboardform->IsValidEdit(lineEdit)) // 不为自己
        {
            mKeyboardform->SetText(lineEdit->text());
            mTextChangeConnection = connect(lineEdit, &QLineEdit::textChanged, mKeyboardform, [=](const QString& text){
                int pos = lineEdit->cursorPosition();
                mKeyboardform->SetText(text);
                lineEdit->setCursorPosition(pos);
            });
//            mTextChangeSignal = connect(mKeyboardform, &KeyBoardForm::SignalInputChanged, lineEdit, &QLineEdit::setText);
            mTextChangeSignal = connect(mKeyboardform, &KeyBoardForm::SignalInputChanged, lineEdit, [=](const QString& text){
                int pos = mKeyboardform->GetCursorPosition();
                lineEdit->setText(text);
                mKeyboardform->SetCursorPosition(pos);
            });
        }
    }
    else if (QTextEdit *textEdit = qobject_cast<QTextEdit*>(focusWidget))
    {
        mKeyboardform->SetText(textEdit->toPlainText());
        mTextChangeConnection = connect(textEdit, &QTextEdit::textChanged, mKeyboardform, [=](){
            // 光标恢复
            QTextCursor cursor = textEdit->textCursor();
            auto pos = textEdit->textCursor().position();
            mKeyboardform->SetText(textEdit->toPlainText());
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, pos);
            textEdit->setTextCursor(cursor);
        });
        mTextChangeSignal = connect(mKeyboardform, &KeyBoardForm::SignalInputChanged, textEdit, [=](const QString& text){
            int pos = mKeyboardform->GetCursorPosition();
            textEdit->setText(text);
            mKeyboardform->SetCursorPosition(pos);
        });
    }
    else if (QPlainTextEdit *plainTextEdit = qobject_cast<QPlainTextEdit*>(focusWidget))
    {
        mKeyboardform->SetText(plainTextEdit->toPlainText());

        mTextChangeConnection = connect(plainTextEdit, &QPlainTextEdit::textChanged, mKeyboardform, [=](){
            mKeyboardform->SetText(plainTextEdit->toPlainText());
            // 设置光标在plainTextEdit末尾
            plainTextEdit->moveCursor(QTextCursor::End);
        });

        mTextChangeSignal = connect(mKeyboardform, &KeyBoardForm::SignalInputChanged, plainTextEdit, &QPlainTextEdit::insertPlainText);
    }


}
