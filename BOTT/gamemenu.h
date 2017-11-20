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

public:
    GameMenu();
    ~GameMenu();
    void addMenuToScene(GameMenuHandler *); // Добавляет элементы меню на сцену, но выставляет setVisible(false) для его элементов
    void setMenuVisible(bool); // Метод, который отображает на сцене (оно уже добавлено на сцену через метод addMenuToScene)
    QVector<QGraphicsPixmapItem *> * getMenuItems(); // Возвращает вектор с пунктами меню
    virtual void processExitAction(); // Метод, который обрабатывает действие выхода из меню
    virtual void processSelectAction(int ) = 0; // Обработчик выбора пункта меню
    virtual void connectWithBuild(QGraphicsPixmapItem * ) = 0;

protected:
    QGraphicsPixmapItem * nameOfMenu;
    GameMenuHandler * parent;
    QVector<QGraphicsPixmapItem *> menuItems;

};

#endif // GAMEMENU_H
