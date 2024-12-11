#include "mainwindow.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QScreen>
#include <QTextEdit>
#include <QWindow>
#include <QInputMethodEvent>
#include <QList>
#include <QDebug>
#include "virtualkeyboard.h"
#include "keyboardbutton.h"

#define TEST
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget widget;
    QMainWindow m;
    VirtualKeyBoard *keyBoard = VirtualKeyBoard::GetInstance();
    keyBoard->InstallKeyBoard();
#ifdef TEST
    // 测试代码
    QVBoxLayout *layout = new QVBoxLayout(&widget);
    layout->addWidget(new QLineEdit);
    layout->addWidget(new QTextEdit);
    layout->addWidget(new QLineEdit);
    layout->addWidget(new QPushButton);
    widget.show();
#endif

    return a.exec();
}
