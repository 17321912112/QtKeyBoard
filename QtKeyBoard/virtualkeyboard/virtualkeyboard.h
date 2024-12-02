#ifndef VIRTUALKEYBOARD_H
#define VIRTUALKEYBOARD_H

#include <QObject>
#include "languagelayout.h"


class VirtualKeyBoard : public QObject
{
    Q_OBJECT
public:
    static VirtualKeyBoard* GetInstance();

    LanguageLayout* GetLayout();

    void InstallKeyBoard(QApplication *app);  // 安装虚拟键盘

    void SetLanguage(int language);  // TODO : 设置键盘语言

    
signals:
    void keyClicked();

private:
    explicit VirtualKeyBoard(QObject *parent = nullptr);

private:
    static VirtualKeyBoard *mKeyBoard; // 虚拟键盘
    class CPrivate;
    CPrivate *const md;
};

#endif // VIRTUALKEYBOARD_H
