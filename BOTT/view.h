#ifndef VIEW_H
#define VIEW_H

/*
 * Это класс, отвечающий за QGraphicsView
*/

#include "enums.h"
#include <QGraphicsView>
#include <QMap>

class Army;
class GameMenuHandler;
class Town;
class Widget;
class QTimer;

class View : public QGraphicsView
{
    Q_OBJECT

    Army * army;
    ViewPosition viewPosition; // Переменная, указывающая позицию view (верхний или нижний view)
    bool canMenuOpen; // Переменная, по которой определяется можно ли открыть меню этого игрока (нельзя, если с момента последнего открытия меню этого игрока не прошло нужное время)
    bool menuOpen; // Указывает, что сейчас меню открыто (эта переменная нужна, т.к. canMenuOpen, указывает на то, что меню можно открыть, но не указывает, что оно открыто)
    GameMenuHandler * gameMenu;
    int inMenuTimer_interval;
    int inMenuTimer_remainingTime; // При выходе в меню таймер остановится и сохранит оставшееся время в переменную, чтобы потом возобновить отсчёт
    int lastSceneX; // Переменная, нужная для нижнего view, которая хранит координату x, с которой view будет отображать сцену
    int pauseMenuTimer_interval;
    int pauseMenuTimer_remainingTime; // При выходе в меню таймер остановится и сохранит оставшееся время в переменную, чтобы потом возобновить отсчёт
    QMap<QString, Qt::Key> controlKeys; // Словарь с клавишами управления этой сценой
    QMap<QString, int> priceUpgrade;
    QMap<QString, Qt::Key> tempSettings; // Словарь с новыми клавишами управления этой сценой (сюда добавляются новые клавиши управления, но если юзер принял новые настройки, то клавиши отсюда удаляются и передаются в controlKeys)
    QTimer * inMenuTimer; // Таймер следит за временем нахождения в меню
    QTimer * pauseMenuTimer; // Таймер следит за паузой между вызовом меню этого view
    Town * town;
    Widget * mainParent;

public:
    View(ViewPosition viewPosition, Widget * parent = nullptr);
    ~View() override;

    Army * getArmy();
    bool event(QEvent * event) override; // QObject interface
    bool isCanMenuOpen();
    bool isControlKey(int key); // Проверка, что нажатая клавиша - клавиша управления игрока
    bool isControlKey(Qt::Key key, SettingMap map = SettingMap::Main);
    bool isControlKey(quint32 key);
    bool isCKContainKeyWithoutCrossingWithTS(Qt::Key key); // CK - QMap<QString, Qt::Key> controlKeys, TS - QMap<QString, Qt::Key> tempSettings
    bool isShortcut(int key); // Проверка, что это горячая клавиша создания юнита
    bool isShortcut(quint32 key);
    GameMenuHandler * getGameMenu();
    int getPriceUpgrade(QString keyInPriceMap);
    QMap<QString, Qt::Key> & getControlKeys();
    QMap<QString, int> * getPriceUpgradeMap();
    QString getValueByControlKey(QString keyInControlMap, SettingMap map = SettingMap::Main);
    Qt::Key getControlKey(QString keyInControlMap); // Возвращает код кнопки по ключу
    Town * getTown();
    void clearTempSettings();
    void clearStart();
    void configureControlKeys(QVector<int> * errors);
    void deleteCurrentMenuItem();
    void replaceCKByTS();
    void setControlKey(QString keyInControlMap, Qt::Key keyValue);
    void setPriceUpgrade(QString keyInPriceMap, int value);
    void setTempControlKey(QString keyInControlMap, Qt::Key keyValue);
    void startAllTimers();
    void stopAllTimers();

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent * event) override;
    void resizeEvent(QResizeEvent * event) override;

private:
    void setConfiguration(); // Настройка игровых меню

private slots:
    void hideMenu(); // Закрывает окно меню

signals:
    void menuVisibleStatusChanged(View * view); // Сигнал, сообщающий виджету, что время нахождения игрока в меню закончилось

};

#endif // VIEW_H
