#ifndef GAMEMENUHANDLER_H
#define GAMEMENUHANDLER_H

/*
 * Это класс, который отвечает за все игровые меню.
 * Он получает задания от view и обрабатывает их, вызывая нужные методы текущего меню через указатель на базовый абстрактный класс.
 * Класс вызывает метод добавления меню на сцену, метод-обработчик выбора пункта меню, закрытия меню
*/

#include <QObject>
#include <QGraphicsPixmapItem>
#include <typeinfo>

class GameMenu;
class MainMenu;
class ScienceMenu;
class WorkshopMenu;
class HospitalMenu;
class MineMenu;
class Army;

class GameMenuHandler : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

    GameMenu * currentOpenMenu; // указатель на базовый абстрактный класс всех меню

    // Игровые меню
    MainMenu * mainMenu;
    ScienceMenu * scienceMenu;
    WorkshopMenu * workshopMenu;
    HospitalMenu * hospitalMenu;
    MineMenu * mineMenu;

    // Элемент, который нужен для выделения текущего пункта меню (золотая рамка)
    QGraphicsPixmapItem * mFocus;

    int currentItem; // Текущий пункт меню

public:
    GameMenuHandler();
    ~GameMenuHandler();
    void addMenusToScene(); // Вызывает метод добавления меню на сцену

    // Методы, в которых соответствующее меню отображается на сцене (оно уже добавлено на сцену через метод addMenusToScene)
    void showMainMenu();
    void showScienceMenu();
    void showWorkshopMenu();
    void showHospitalMenu();
    void showMineMenu();

    void setCurrentItem(bool); // Метод выделения пункта меню
    void hideCurrentOpenMenu(); // Скрыть открытое меню
    void processSelectAction(); // Метод, который вызывает обработчик выбора пункта соответствующего меню
    void processExitAction(); // Метод, который обрабатывает действие выхода из текущего меню

    void connectToMenus(QObject *);

signals:
    void closeMenu();

};

#endif // GAMEMENUHANDLER_H
