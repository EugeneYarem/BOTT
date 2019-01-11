#ifndef GAMEMENU_H
#define GAMEMENU_H

/*
 * Это абстрактный класс.
 * Он является базовым для всех игровых меню.
 * Отвечает за добавление меню на сцену, отображения меню и обработку выхода из текущего меню
*/

#include <QObject>
#include <QGraphicsPixmapItem>

class GameMenuHandler;

class GameMenu : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

protected:
    GameMenuHandler * parent;
    QGraphicsPixmapItem * nameOfMenu;
    QVector<QGraphicsPixmapItem *> menuItems;

public:
    GameMenu();
    ~GameMenu();

    QGraphicsPixmapItem * getNameOfMenu();
    QVector<QGraphicsPixmapItem *> * getMenuItems(); // Возвращает вектор с пунктами меню
    virtual int getPriceOfCurrentItem(QMap<QString, int> * priceMap, int currentItem) = 0;
    virtual void connectWithObject(QObject * objectForConnect) = 0;
    virtual void processExitAction(); // Метод, который обрабатывает действие выхода из меню
    virtual void processSelectAction(int currentItem) = 0; // Обработчик выбора пункта меню
    void addMenuToScene(GameMenuHandler * parent); // Добавляет элементы меню на сцену, но выставляет setVisible(false) для его элементов
    void setMenuVisible(bool visible); // Метод, который отображает на сцене (оно уже добавлено на сцену через метод addMenuToScene)
    void setNameOfMenu(QGraphicsPixmapItem * item);

};

#endif // GAMEMENU_H
