#ifndef GAMEMENUHANDLER_H
#define GAMEMENUHANDLER_H

/*
 * Это класс, который отвечает за все игровые меню.
 * Он получает задания от view и обрабатывает их, вызывая нужные методы текущего меню через указатель на базовый абстрактный класс.
 * Класс вызывает метод добавления меню на сцену, метод-обработчик выбора пункта меню, закрытия меню
*/

#include "enums.h"
#include <QGraphicsPixmapItem>
#include <QObject>

class GameMenu;
class HospitalMenu;
class MagicMenu;
class MainMenu;
class MineMenu;
class WorkshopMenu;
class View;

class GameMenuHandler : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    ConflictSide side; // Сторона меню (нужно для ценника)
    int currentItem; // Текущий пункт меню
    GameMenu * currentOpenMenu; // указатель на базовый абстрактный класс всех меню

    // Игровые меню
    HospitalMenu * hospitalMenu;
    MagicMenu * magicMenu;
    MainMenu * mainMenu;
    MineMenu * mineMenu;

    // Элемент, который нужен для выделения текущего пункта меню (золотая рамка)
    QGraphicsPixmapItem * mFocus;
    // Элемент, который нужен для вывода цены текущего пункта
    QGraphicsPixmapItem * mPrice;
    QGraphicsTextItem * priceItem;
    View * parent; // Указатель на родительскую сцену
    WorkshopMenu * workshopMenu; // Игровое меню

public:
    GameMenuHandler(View * parent);
    ~GameMenuHandler();

    View * getParentView() const;
    void addMenusToScene(); // Вызывает метод добавления меню на сцену
    void connectToMenus(const QObject * objectForConnect) const;
    void deleteCurrentMenuItem();
    void hideCurrentOpenMenu(); // Скрыть открытое меню
    void processExitAction() const; // Метод, который обрабатывает действие выхода из текущего меню
    void processSelectAction(); // Метод, который вызывает обработчик выбора пункта соответствующего меню
    void restoreLastGame(const QMap<QString, int> & restorePriceUpgradeMap);
    void setCurrentItem(const DirectionInMenu & upOrDown); // Метод выделения пункта меню; true - вверх, false - вниз
    void setPriceSide(const ConflictSide & side); // Устанавливает сторону для вывода цены улучшения

    // Методы, в которых соответствующее меню отображается на сцене (оно уже добавлено на сцену через метод addMenusToScene)
    void showHospitalMenu();
    void showMagicMenu();
    void showMainMenu();
    void showMineMenu();
    void showWorkshopMenu();

private:
    void setFocusAndPricePos(const qreal & xF, const qreal & xP, const qreal & y) const;

signals:
    void closeMenu();

};

#endif // GAMEMENUHANDLER_H
