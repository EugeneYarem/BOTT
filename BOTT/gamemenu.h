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

    QGraphicsPixmapItem * getNameOfMenu() const;
    QVector<QGraphicsPixmapItem *> * getMenuItems(); // Возвращает вектор с пунктами меню
    virtual int getPriceOfCurrentItem(const QMap<QString, int> * priceMap, const int & currentItem) const = 0;
    virtual QVector<int> restoreLastGame(const QMap<QString, int> & restorePriceUpgradeMap) const = 0;
    virtual void connectWithObject(const QObject * objectForConnect) const = 0;
    virtual void processExitAction() const; // Метод, который обрабатывает действие выхода из меню
    virtual void processSelectAction(const int & currentItem) = 0; // Обработчик выбора пункта меню
    void addMenuToScene(GameMenuHandler * parent); // Добавляет элементы меню на сцену, но выставляет setVisible(false) для его элементов
    void setMenuVisible(const bool & visible) const; // Метод, который отображает на сцене (оно уже добавлено на сцену через метод addMenuToScene)
    void setNameOfMenu(QGraphicsPixmapItem * item);

};

#endif // GAMEMENU_H
