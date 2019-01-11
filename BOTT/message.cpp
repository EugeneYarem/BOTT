#include "message.h"
#include "ui_message.h"


Message::Message(QWidget * parent) :
    QDialog(parent),
    ui(new Ui::Message)
{
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

void Message::on_pushButton_released()
{
    close();
    emit okButtonPress();
}
