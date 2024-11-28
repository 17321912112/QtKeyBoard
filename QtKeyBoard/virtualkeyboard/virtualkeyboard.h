#ifndef VIRTUALKEYBOARD_H
#define VIRTUALKEYBOARD_H

#include <QObject>
#include "languagelayout.h"


class VirtualKeyBoard : public QObject
{
    Q_OBJECT
public:
    
    static VirtualKeyBoard* GetInstance();

    languageLayout* GetLayout();

    void InstallKeyBoard(QApplication *app);  // 安装虚拟键盘

signals:
    void send_preedit(const QString &text);

    void send_commit(const QString &text);

    void keyClicked();

private:
    explicit VirtualKeyBoard(QObject *parent = nullptr);

private:
    static VirtualKeyBoard *mKeyBoard; // 虚拟键盘
    class CPrivate;
    CPrivate *const md;
};

#endif // VIRTUALKEYBOARD_H
