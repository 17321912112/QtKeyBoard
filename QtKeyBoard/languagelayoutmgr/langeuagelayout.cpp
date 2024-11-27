#include "langeuagelayout.h"
#include "ui_keyboarddialog.h"

class LangeuageLayout::CPrivate
{
public:
    CPrivate(LangeuageLayout* parent)
        : mParent(parent)
    {
        ui->setupUi(parent);
    }

    LangeuageLayout* mParent;
    Ui::KeyBoardForm *ui;
};


LangeuageLayout::LangeuageLayout(QDialog *parent)
    : QDialog(parent)
    , md(new CPrivate(this))
{

}
