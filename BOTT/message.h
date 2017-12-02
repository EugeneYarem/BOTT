#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDialog>

namespace Ui {
class Message;
}

class Message : public QDialog
{
    Q_OBJECT

    bool isNewGameMes;
    bool isExitMes;

public:
    explicit Message(QWidget *parent = 0);
    ~Message();
    void setMessage(QString);
    void setNewGameStatus();
    void setExitStatus();

private slots:
    void on_pushButton_released();

private:
    Ui::Message *ui;

};

#endif // MESSAGE_H
