#include "message.h"
#include "ui_message.h"


Message::Message(QWidget * parent) :
    QDialog(parent),
    ui(new Ui::Message)
{
    isButtonOkPressed = false;
    ui->setupUi(this);
}

Message::~Message()
{
    delete ui;
}

void Message::setMessage(QString str)
{
    ui->label->setText(str);
}

void Message::closeEvent(QCloseEvent *)
{
    emit end(isButtonOkPressed);
}

void Message::on_pushButton_released()
{
    isButtonOkPressed = true;
    close();
    emit okButtonPress();  
}
