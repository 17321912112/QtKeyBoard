#ifndef LANGEUAGELAYOUT_H
#define LANGEUAGELAYOUT_H

#include <QObject>
#include <QDialog>

namespace Ui {
class LangeuageLayout;
}

class LanguageLayout : public QWidget
{
    Q_OBJECT
public:
    explicit LanguageLayout(QWidget *parent = nullptr);

    void SetCurrentLanguage(QString language); // TODO

protected:
    void showEvent(QShowEvent *event) override;

    void hideEvent(QHideEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override; // 监听系统某些按键是否被按下

    void keyPressEvent(QKeyEvent *event) override; // 监听系统某些按键是否被按下

signals:
    // 大小写布局切换,通知按钮改变显示按键
    void CapsLockSwitched(bool state); 
    // Shift切换,通知按钮改变显示按键
    void ShiftSwitched(bool state);    

public slots:
    void SlotCapsSwitch(void);
    void SlotShiftSwitch(void);
    void SlotKeyClicked(int keyCode);
    void SlotSwitchLanguage();

private:
    void ShiftPress();
    void CapsLockPress();
    void ShiftRelease();
    void CapsLockRelease();
private:
    class CPrivate;
    CPrivate *const md;
};

#endif // LANGEUAGELAYOUT_H
