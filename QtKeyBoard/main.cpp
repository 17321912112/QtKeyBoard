#include "mainwindow.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QScreen>
#include <QTextEdit>
#include <QWindow>
#include "virtualkeyboard/virtualkeyboard.h"
#include "keyboardbutton.h"

#define TEST
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef TEST
    // 测试代码
    QWidget widget;
    QVBoxLayout *layout = new QVBoxLayout(&widget);
    layout->addWidget(new QLineEdit);
    layout->addWidget(new QTextEdit);
    layout->addWidget(new QLineEdit);
    widget.show();

    QObject::connect(qApp->inputMethod(), &QInputMethod::cursorRectangleChanged,
                     VirtualKeyBoard::GetInstance(), [](){
        QWindow *focusWindow = qApp->focusWindow();
        if (focusWindow && qApp->focusWidget() && !VirtualKeyBoard::GetInstance()->isVisible()) {
            QRect rect = qApp->inputMethod()->cursorRectangle().toRect().translated(focusWindow->position());
			QPoint pos = rect.bottomLeft() + QPoint(0, 5);
            QScreen *screen = qApp->screenAt(pos);
			if (screen == Q_NULLPTR)
				screen = qApp->primaryScreen();

            if (pos.x() + VirtualKeyBoard::GetInstance()->width() > screen->geometry().width())
                pos.setX(screen->geometry().width() - VirtualKeyBoard::GetInstance()->width());
            if (pos.y() + VirtualKeyBoard::GetInstance()->height() > screen->geometry().height())
                pos.setY(screen->geometry().height() - VirtualKeyBoard::GetInstance()->height());

            VirtualKeyBoard::GetInstance()->move(pos);
            VirtualKeyBoard::GetInstance()->show();
        }
    });
#endif
    MainWindow w;
    w.show();
    return a.exec();
}
