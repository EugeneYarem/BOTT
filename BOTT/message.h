#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDialog>

namespace Ui {
    class Message;
}

class Message : public QDialog
{
    Q_OBJECT

    bool isButtonOkPressed;
    Ui::Message *ui;

public:
    explicit Message(QWidget * parent = nullptr);
    ~Message() override;

    void setMessage(const QString & text);

    // QWidget interface
protected:
    void closeEvent(QCloseEvent * event) override;

private slots:
    void on_pushButton_released();

signals:
    /*
     * Два сигнала являются похожими. Они оба могут сообщать, что "Ок" была нажата.
     * Сигнал end используется в closeEvent. При создании коннектов к объекту данного класса в widget.cpp, в некоторых местах
     * необъодимо выполнить действия и по закрытию сообщения, и по нажатию кнопки и выполниться они должны в четкой последовательности
     * (обработать нажатие кнопки, потом обработать закрытие). В других же местах закрыть сообщение нужно до отправки сигнала о нажатии кнопки Ок.
     * Для понимания смотреть функции void Widget::showMessageAboutUnsavedSettings() и void Widget::on_buttonNew_pressed().
    */
    void end(const bool & isButtonOkPressed);
    void okButtonPress();

};

#endif // MESSAGE_H
