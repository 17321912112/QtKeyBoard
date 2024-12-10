#ifndef INPUTFORM_H
#define INPUTFORM_H

#include <QWidget>
#include <QLineEdit>

namespace Ui {
class InputForm;
}

class InputForm : public QWidget
{
    Q_OBJECT

public:
    explicit InputForm(QWidget *parent = nullptr);
    ~InputForm();
public:
    void SetText(const QString& text);

    bool IsValide(QLineEdit *edit);

    void SetCursorPosition(int pos);

    int GetCursorPosition();

signals:
    void SignalTextChanged(const QString& text);
private:
    void Init();
private:
    Ui::InputForm *ui;
};

#endif // INPUTFORM_H
