#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDialog>

namespace Ui {
    class Message;
}

class Message : public QDialog
{
    Q_OBJECT

    Ui::Message *ui;

public:
    explicit Message(QWidget *parent = nullptr);
    ~Message();

    void setMessage(QString text);

private slots:
    void on_pushButton_released();

signals:
    void okButtonPress();

};

#endif // MESSAGE_H
