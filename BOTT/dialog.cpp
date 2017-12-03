#include "dialog.h"
#include "ui_dialog.h"
#include "widget.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    ui->label->setStyleSheet("QLabel{background: rgba(255, 255, 255, 190); padding-left: 5px; padding-right: 5px}");
    ui->label_2->setStyleSheet("QLabel{background: rgba(255, 255, 255, 190); padding-left: 5px; padding-right: 5px}");
    ui->label_3->setStyleSheet("QLabel{background: rgba(255, 255, 255, 190); padding-left: 5px; padding-right: 5px}");
    ui->lineEdit->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 190); padding-left: 5px; padding-right: 5px}");
    ui->lineEdit_2->setStyleSheet("QLineEdit{background: rgba(255, 255, 255, 190); padding-left: 5px; padding-right: 5px}");
    ui->pushButton->setStyleSheet("QPushButton{background: rgba(255, 255, 255, 190); padding-left: 5px; padding-right: 5px}");

    ui->pushButton->setDefault(true);

    ((Widget *)this->parent())->setIsStartDialogOpen();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_pressed()
{
    if(ui->lineEdit->text().length() == 0 || ui->lineEdit_2->text().length() == 0)
        return;
    close();
    ((Widget *)this->parent())->setGamerNameP1(ui->lineEdit->text());
    ((Widget *)this->parent())->setGamerNameP2(ui->lineEdit_2->text());
    ((Widget *)this->parent())->startNewGame();
}
