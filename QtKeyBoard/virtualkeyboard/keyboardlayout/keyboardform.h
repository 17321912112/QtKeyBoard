#ifndef KEYBOARDFORM_H
#define KEYBOARDFORM_H

#include <QWidget>
#include <QLineEdit>
#include "inputmethod.h"

namespace Ui
{
    class KeyBoardForm;
}

class KeyBoardForm : public QWidget
{
    Q_OBJECT
public:
    explicit KeyBoardForm(QWidget *parent = nullptr);
public:
    void SetLanguage(KeyBoard::Language language);

    /*--同步输入框--*/
    void SetText(const QString& text);

    bool IsValidEdit(QLineEdit *edit);

    void SetCursorPosition(int pos);

    int GetCursorPosition();
    /*--同步输入框--*/

public slots:
    void SlotNewCharacter(const QString& text);

    void SlotSwitchLayout();
protected:
    void showEvent(QShowEvent *event) override;

    void hideEvent(QHideEvent *event) override;

signals:
    void SignalInputChanged(const QString& text);

    void SignalCommited(const QString& text);
private:
    class CPrivate;
    CPrivate *const md;
};

#endif // KEYBOARDFORM_H
