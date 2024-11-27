#ifndef VIRTUALKEYBOARD_H
#define VIRTUALKEYBOARD_H

#include <QObject>
#include "languagelayoutmgr/langeuagelayout.h"

class VirtualKeyBoard : public QObject
{
    Q_OBJECT
public:
    explicit VirtualKeyBoard(QObject *parent = nullptr);

    static LangeuageLayout* GetInstance();

signals:


private:

    static LangeuageLayout *mKeyBoard; // 虚拟键盘
    class CPrivate;
    CPrivate *const md;
};

#endif // VIRTUALKEYBOARD_H
