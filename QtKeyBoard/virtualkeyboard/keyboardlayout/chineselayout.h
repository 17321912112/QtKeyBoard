// chineselayout.h
#ifndef CHINESELAYOUT_H
#define CHINESELAYOUT_H

#include "abstractlayout.h"

namespace Ui {
class ChineseLayout;
}

class ChineseLayout : public AbstractLayout
{
    Q_OBJECT
public:
    explicit ChineseLayout(QWidget *parent = nullptr);

    // void SetCurrentLanguage(QString language) override;

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

public slots:
    void SlotCapsSwitch(void) override;
    void SlotShiftSwitch(void) override;
    void SlotKeyClicked(int keyCode) override;
    void SlotSwitchLanguage() override;

private:
    void ShiftPress() override;
    void CapsLockPress() override;
    void ShiftRelease() override;
    void CapsLockRelease() override;

private:
    class CPrivate;
    CPrivate *const md;
};

#endif // CHINESELAYOUT_H