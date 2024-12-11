#ifndef VIRTUALKEYBOARD_H
#define VIRTUALKEYBOARD_H

#include <QObject>
#include <QMouseEvent>
#include "inputmethod.h"
#include "keyboardform.h"

class VirtualKeyBoard;

// 事件过滤器
class MouseEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit MouseEventFilter(QObject *parent = nullptr);
    void setKeyboard(VirtualKeyBoard *keyboard);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:

    VirtualKeyBoard *m_keyboard;
};

class VirtualKeyBoard : public QObject
{
    Q_OBJECT
public:
    static VirtualKeyBoard* GetInstance();

    // 安装虚拟键盘
    void InstallKeyBoard();
    // 设置键盘语言
    void SetLanguage(KeyBoard::Language language); 

    friend class MouseEventFilter;

signals:

private:
    explicit VirtualKeyBoard(QObject *parent = nullptr);

private:
    void InitKeyboardWidget();

    void LoadDefaultLayout();

    void SyncText(QWidget *focusWidget);

    bool IsEditWidget(QWidget *focusWidget);

    void MoveAndShow();

    KeyBoardForm* GetKeyBoardForm();

private:
    static VirtualKeyBoard *mKeyBoard; // 虚拟键盘
    KeyBoardForm* mKeyboardform;
    MouseEventFilter *m_eventFilter; // 添加事件过滤器指针
    QMetaObject::Connection mTextChangeConnection; // 存储预输入正向连接
    QMetaObject::Connection mTextChangeSignal; // 存储预输入反连接
};

#endif // VIRTUALKEYBOARD_H
