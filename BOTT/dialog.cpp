#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

QString Dialog::getName1()
{
    return ui->lineEdit->text();
}

QString Dialog::getName2()
{
    return ui->lineEdit_2->text();
}
