#ifndef LANGEUAGELAYOUT_H
#define LANGEUAGELAYOUT_H

#include <QObject>
#include <QDialog>

namespace Ui {
class LangeuageLayout;
}

class LangeuageLayout : public QDialog
{
    Q_OBJECT
public:
    explicit LangeuageLayout(QDialog *parent = nullptr);

signals:

private:
    class CPrivate;
    CPrivate *const md;
};

#endif // LANGEUAGELAYOUT_H
