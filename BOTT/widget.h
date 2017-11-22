#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
    class Widget;
}

class View;
class Battlefield;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void updateViewWithOpenMenu(View * );

private:
    Ui::Widget *ui;
    View * view; // Верхний view
    View * view_2; // Нижний view
    View * viewWithOpenMenu; // view, у которого активно меню
    Battlefield * btf;
    void createSettingsPage();

    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // WIDGET_H
