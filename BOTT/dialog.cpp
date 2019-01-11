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

void Dialog::on_pushButton_pressed()
{
    if(ui->lineEdit->text().length() == 0 || ui->lineEdit_2->text().length() == 0)
        return;
    emit startNewGame(ui->lineEdit->text(), ui->lineEdit_2->text());
    close();    
}

void Dialog::closeEvent(QCloseEvent * )
{
    emit dialogIsOpen(false);
}
