#ifndef CHARACTERFORM_H
#define CHARACTERFORM_H

#include <QWidget>

namespace Ui {
class CharacterForm;
}

class CharacterForm : public QWidget
{
    Q_OBJECT

public:
    explicit CharacterForm(QWidget *parent = nullptr);
    ~CharacterForm();

signals:
    void NewCharacterClicked(const QString& text);
    void BackClicked();

private:
    void Init();
private:
    Ui::CharacterForm *ui;
};

#endif // CHARACTERFORM_H
