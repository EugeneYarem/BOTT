#include "message.h"
#include "ui_message.h"
#include "widget.h"

Message::Message(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Message)
{
    ui->setupUi(this);
    ui->pushButton->setDefault(true);

    ui->label->setStyleSheet("QLabel{background: rgba(255, 255, 255, 190); padding-left: 5px; padding-right: 5px}");
    ui->pushButton->setStyleSheet("QPushButton{background: rgba(255, 255, 255, 190); padding-left: 5px; padding-right: 5px}");

    isNewGameMes = false;
    isExitMes = false;
}

Message::~Message()
{
    delete ui;
}

void Message::setMessage(QString str)
{
    ui->label->setText(str);
}

void Message::setNewGameStatus()
{
    isNewGameMes = true;
}

void Message::setExitStatus()
{
    isExitMes = true;
}

void Message::on_pushButton_released()
{
    if(isNewGameMes)
    {
        close();
        ((Widget *)parent())->showStartDialog();
        return;
    }

    if(isExitMes)
    {
        close();
        ((Widget *)parent())->exitFromGame();
        ((Widget *)parent())->save();
        return;
    }

    close();
}
