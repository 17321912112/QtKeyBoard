#ifndef VIRTUALKEYBOARD_H
#define VIRTUALKEYBOARD_H

#include <QObject>
#include "inputmethod.h"
#include "abstractlayout.h"
#include "keyboardlayoutfactory.h"

class VirtualKeyBoard : public QObject
{
    Q_OBJECT
public:
    static VirtualKeyBoard* GetInstance();

    AbstractLayout* GetLayout();

    void InstallKeyBoard(QApplication *app);  // 安装虚拟键盘

    void SetLanguage(KeyBoard::Language language);

signals:
    // void keyClicked();

private:
    explicit VirtualKeyBoard(QObject *parent = nullptr);

private:
    static VirtualKeyBoard *mKeyBoard; // 虚拟键盘
    AbstractLayout* mLayout;
};

#endif // VIRTUALKEYBOARD_H
