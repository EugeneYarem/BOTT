#ifndef MAINMENU_H
#define MAINMENU_H

#include "gamemenu.h"

class GameMenuHandler;

class MainMenu : public GameMenu
{
    Q_OBJECT

public:
    MainMenu();

    // GameMenu interface
public:
    void processSelectAction(int );
    void processExitAction();
    void connectWithBuild(QGraphicsPixmapItem *);
};

#endif // MAINMENU_H
