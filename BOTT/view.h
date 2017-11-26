#ifndef VIEW_H
#define VIEW_H

/*
 * Это класс, отвечающий за QGraphicsView
*/

#include <QGraphicsView>
#include <QMap>

class Town;
class Army;
class GameMenuHandler;
class QTimer;

class View : public QGraphicsView
{
    Q_OBJECT

    GameMenuHandler * gameMenu;
    Town * town;
    Army * army;
    QTimer * inMenuTimer; // Таймер следит за временем нахождения в меню
    QTimer * pauseMenuTimer; // Таймер следит за паузой между вызовом меню этого view
    int inMenuTimer_remainingTime; // При выходе в меню таймер остановится и сохранит оставшееся время в переменную, чтобы потом возобновить отсчёт
    int pauseMenuTimer_remainingTime; // При выходе в меню таймер остановится и сохранит оставшееся время в переменную, чтобы потом возобновить отсчёт
    bool bottomView; // Переменная, указывающая, что это нижняя сцена на виджете
    int lastSceneX; // Переменная, нужная для нижнего view, которая хранит координату x, с которой view будет отображать сцену
    QMap<QString, Qt::Key> controlKeys; // Словарь с клавишами управления этой сценой
    bool canMenuOpen; // Переменная, по которой определяется можно ли открыть меню этого игрока (нельзя, если с момента последнего открытия меню этого игрока не прошло нужное время)
    bool menuOpen; // Указывает, что сейчас меню открыто (эта переменная нужна, т.к. canMenuOpen, указывает на то, что меню можно открыть, но не указывает, что оно открыто)
    QMap<QString, int> priceUpgrade;

public:
    View(bool bottomView, QWidget * parent = 0);
    ~View();
    void setConfiguration(); // Настройка кнопок управления и игровых меню
    bool isCanMenuOpen();
    bool isControlKey(quint32); // Проверка, что нажатая клавиша - клавиша управления игрока
    bool isControlKey(int);
    bool isShortcut(quint32); // Проверка, что это горячая клавиша создания юнита
    bool isShortcut(int);
    Qt::Key getControlKey(QString key); // Возвращает код кнопки по ключу
    QString getValueByControlKey(QString);
    void setControlKey(QString, Qt::Key);
    void setPriceUpgrade(QString, int);
    int getPriceUpgrade(QString key);
    bool checkControlKey(Qt::Key);
    Army * getArmy();
    Town * getTown();
    void stopAllTimers();
    void startAllTimers();

private slots:
    void hideMenu(); // Закрывает окно меню
    void setCanMenuOpenInTrue();

signals:
    void menuVisibleStatusChanged(View *); // Сигнал, сообщающий виджету, что время нахождения игрока в меню закончилось
    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);

    // QObject interface
public:
    bool event(QEvent *event);   
};

#endif // VIEW_H
