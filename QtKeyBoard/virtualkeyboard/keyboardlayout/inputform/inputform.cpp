#include "inputform.h"
#include "ui_inputform.h"

InputForm::InputForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputForm)
{
    ui->setupUi(this);
    Init();
}

InputForm::~InputForm()
{
    delete ui;
}

void InputForm::SetText(const QString &text)
{
    ui->lineEdit->setText(text);
}

bool InputForm::IsValide(QLineEdit *edit)
{
    return edit != ui->lineEdit;
}

void InputForm::SetCursorPosition(int pos)
{
    ui->lineEdit->setCursorPosition(pos);
}

int InputForm::GetCursorPosition()
{
    return ui->lineEdit->cursorPosition();
}

void InputForm::Init()
{
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &InputForm::SignalTextChanged);
}
