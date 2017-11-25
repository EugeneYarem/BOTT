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

    Ui::Widget *ui;
    View * view; // Верхний view
    View * view_2; // Нижний view
    View * viewWithOpenMenu; // view, у которого активно меню
    Battlefield * btf;
    int lastVisitedPage;
    bool eventEvoke; // Переменная нужна для того, чтобы определить было ли обработано событие нажатие клавиши Esc одним из view.

    void createSettingsPage();
    bool isSettingLineEdit(QObject *);
    void installEventFilters();
    bool isLineEditOfFirstPlayer(QObject * );
    void clearFocusOfMainMenu();
    void stopAllTimers();
    void startAllTimers();

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_buttonSettings_pressed();
    void on_buttonContinue_pressed();
    void on_buttonNew_pressed();
    void on_buttonExit_pressed();

public slots:
    void updateViewWithOpenMenu(View * );

    // QObject interface
public:
    bool event(QEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

};

#endif // WIDGET_H
