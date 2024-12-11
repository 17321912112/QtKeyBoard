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

void MouseEventFilter::setKeyboard(VirtualKeyBoard *keyboard)
{
    m_keyboard = keyboard;
}

bool MouseEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) 
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        // 如果当前点击的是输入框，呼出键盘
        if (m_keyboard) 
        {
            if (!m_keyboard->GetKeyBoardForm()->isVisible() && m_keyboard->IsEditWidget(qApp->widgetAt(mouseEvent->globalPos())))
            {
                m_keyboard->MoveAndShow();
            }
            else if (m_keyboard->GetKeyBoardForm()->isVisible() && !m_keyboard->GetKeyBoardForm()->InGeometry(mouseEvent->globalPos()))
            {
                m_keyboard->GetKeyBoardForm()->hide();
            }
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


void VirtualKeyBoard::InstallKeyBoard()
{
    InitKeyboardWidget();

    qApp->installEventFilter(m_eventFilter); // 安装事件过滤器

    // 计算位置 呼出键盘
    QObject::connect(qApp->inputMethod(), &QInputMethod::cursorRectangleChanged, this, [&](){
            MoveAndShow();
       });

    // 字符显示
    QObject::connect(mKeyboardform, &KeyBoardForm::SignalCommited, [=](const QString &text){
        QInputMethodEvent *event = new QInputMethodEvent;
        event->setCommitString(text);
        QWidget *focusWindow = qApp ? qApp->focusWidget() : Q_NULLPTR;
        if (focusWindow)
        {
            qApp->postEvent(focusWindow, event);
        }
    });

}

void VirtualKeyBoard::InitKeyboardWidget()
{
    if (mKeyboardform)
    {
        m_eventFilter->setKeyboard(this);
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

bool VirtualKeyBoard::IsEditWidget(QWidget *focusWidget)
{
    bool res = false;

    if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(focusWidget))
    {
        if (mKeyboardform->IsValidEdit(lineEdit))  // 不为自己
        {
            res = true;
        }
    }
    else if (QTextEdit* textEdit = qobject_cast<QTextEdit*>(focusWidget))
    {
        Q_UNUSED(textEdit)
        res = true;
    }
    else if (QPlainTextEdit* plainEdit = qobject_cast<QPlainTextEdit*>(focusWidget))
    {
        Q_UNUSED(plainEdit)
        res = true;
    }
    return res;
}

void VirtualKeyBoard::MoveAndShow()
{
    if (!IsEditWidget(qApp->focusWidget()) || !qApp->focusWindow()) return ; // 如果不是输入框则忽略

    SyncText(qApp->focusWidget()); // 同步输入框到键盘预输入处

    QWindow *focusWindow = qApp->focusWindow();
    QRect rect = qApp->inputMethod()->cursorRectangle().toRect().translated(focusWindow->position()); // 获取光标位置
    QPoint pos = rect.bottomLeft() + QPoint(0, 30); //
    QScreen *screen = qApp->screenAt(pos); // 获取当前屏幕
    if (screen == Q_NULLPTR)
        screen = qApp->primaryScreen();

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
}
