#ifndef LANGEUAGELAYOUT_H
#define LANGEUAGELAYOUT_H

#include <QObject>
#include <QDialog>

namespace Ui {
class LangeuageLayout;
}

class languageLayout : public QWidget
{
    Q_OBJECT
public:
    explicit languageLayout(QWidget *parent = nullptr);

    void SetCurrentLanguage(QString language); // TODO

signals:
    void CapsLockSwitched(bool state); // 大小写布局切换
    void ShiftSwitched(bool state);    // Shift切换

private:
    class CPrivate;
    CPrivate *const md;
};

#endif // LANGEUAGELAYOUT_H
