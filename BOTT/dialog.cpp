#include "dialog.h"
#include "ui_dialog.h"
#include "widget.h"

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
    ((Widget *)this->parent())->setGamerNameP1(ui->lineEdit->text());
    ((Widget *)this->parent())->setGamerNameP2(ui->lineEdit_2->text());
    ((Widget *)this->parent())->startNewGame();
    close();
}
