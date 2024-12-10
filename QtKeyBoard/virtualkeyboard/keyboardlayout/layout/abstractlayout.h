// abstractlayout.h
#ifndef ABSTRACTLAYOUT_H
#define ABSTRACTLAYOUT_H

#include <QWidget>
#include <QShowEvent>
#include <QHideEvent>
#include <QKeyEvent>

namespace Ui
{
    class AbstractLayout;
}

class AbstractLayout : public QWidget
{
    Q_OBJECT
public:
    explicit AbstractLayout(QWidget *parent = nullptr);

protected:
    virtual void showEvent(QShowEvent *event) override;
    virtual void hideEvent(QHideEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

signals:
    void CapsLockSwitched(bool state);
    void ShiftSwitched(bool state);
    void BackClicked();


public slots:
    virtual void SlotCapsSwitch(void) = 0;
    virtual void SlotShiftSwitch(void) = 0;
    virtual void SlotKeyClicked(int keyCode) = 0;
    virtual void SlotSwitchLanguage() = 0;

private:
    virtual void ShiftPress() = 0;
    virtual void CapsLockPress() = 0;
    virtual void ShiftRelease() = 0;
    virtual void CapsLockRelease() = 0;
};
#endif // ABSTRACTLAYOUT_H


