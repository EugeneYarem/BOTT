#ifndef VIEW_H
#define VIEW_H

/*
 * Это класс, отвечающий за QGraphicsView
*/

#include <QGraphicsView>
#include <QMap>

class GameMenuHandler;
class QTimer;

class View : public QGraphicsView
{
    Q_OBJECT

    GameMenuHandler * gameMenu;
    GameMenuHandler * gameMenu_2;
    QTimer * inMenuTimer; // Таймер следит за временем нахождения в меню
    QTimer * pauseMenuTimer; // Таймер следит за паузой между вызовом меню этого view
    bool bottomView; // Переменная, указывающая, что это нижняя сцена на виджете
    int lastSceneX; // Переменная, нужная для нижнего view, которая хранит координату x, с которой view будет отображать сцену
    QMap<QString, Qt::Key> controlKeys; // Словарь с клавишами управления этой сценой
    bool canMenuOpen; // Переменная, по которой определяется можно ли открыть меню этого игрока (нельзя, если с момента последнего открытия меню этого игрока не прошло нужное время)
    bool menuOpen; // Указывает, что сейчас меню открыто (эта переменная нужна, т.к. canMenuOpen, указывает на то, что меню можно открыть, но не указывает, что оно открыто)

public:
    View(bool bottomView, QWidget * parent = 0);
    ~View();
    void setConfiguration(); // Настройка кнопок управления и игровых меню
    bool isCanMenuOpen();
    bool isControlKey(quint32); // Проверка, что нажатая клавиша - клавиша управления игрока
    bool isControlKey(int); // Проверка, что нажатая клавиша - клавиша управления игрока
    Qt::Key getControlKey(QString key); // Возвращает код кнопки по ключу

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
