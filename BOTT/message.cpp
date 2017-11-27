#include "message.h"
#include "ui_message.h"

Message::Message(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Message)
{
    ui->setupUi(this);
    ui->pushButton->setDefault(true);

    ui->label->setStyleSheet("QLabel{background: rgba(255, 255, 255, 190); padding-left: 5px; padding-right: 5px}");
    ui->pushButton->setStyleSheet("QPushButton{background: rgba(255, 255, 255, 190); padding-left: 5px; padding-right: 5px}");
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
}
