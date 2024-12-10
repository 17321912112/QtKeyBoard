#include "inputform.h"
#include "ui_inputform.h"

InputForm::InputForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputForm)
{
    ui->setupUi(this);
}

InputForm::~InputForm()
{
    delete ui;
}
