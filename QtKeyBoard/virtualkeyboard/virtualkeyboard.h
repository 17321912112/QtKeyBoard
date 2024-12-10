#ifndef VIRTUALKEYBOARD_H
#define VIRTUALKEYBOARD_H

#include <QObject>
#include <QMouseEvent>
#include "inputmethod.h"
#include "keyboardform.h"

// 事件过滤器
class MouseEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit MouseEventFilter(QObject *parent = nullptr);
    void setKeyboard(QWidget *keyboard);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QWidget *m_keyboard;
};

class VirtualKeyBoard : public QObject
{
    Q_OBJECT
public:
    static VirtualKeyBoard* GetInstance();

    // 安装虚拟键盘
    void InstallKeyBoard(QApplication *app);  
    // 设置键盘语言
    void SetLanguage(KeyBoard::Language language); 

signals:

private:
    explicit VirtualKeyBoard(QObject *parent = nullptr);

private:
    KeyBoardForm* GetKeyBoardForm();

    void InitKeyboardWidget();

    void LoadDefaultLayout();

    void SyncText(QWidget *focusWidget);

private:
    static VirtualKeyBoard *mKeyBoard; // 虚拟键盘
    KeyBoardForm* mKeyboardform;
    MouseEventFilter *m_eventFilter; // 添加事件过滤器指针
    QMetaObject::Connection mTextChangeConnection; // 存储预输入正向连接
    QMetaObject::Connection mTextChangeSignal; // 存储预输入反连接
};

#endif // VIRTUALKEYBOARD_H
