#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

    Ui::Dialog * ui;

public:
    explicit Dialog(QWidget * parent = nullptr);
    ~Dialog() override;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent * event) override;

private slots:
    void on_pushButton_pressed();

signals:
    void dialogIsOpen(bool isOpen);
    void startNewGame(QString gamer1, QString gamer2);

};

#endif // DIALOG_H
