#ifndef MINEMENU_H
#define MINEMENU_H

#include "gamemenu.h"

class GameMenuHandler;

class MineMenu : public GameMenu
{
    Q_OBJECT

public:
    MineMenu();

signals:
    // M - Mine
    void M_LevelUp();

    // GameMenu interface
public:
    void processSelectAction(int );
    void connectWithBuild(QGraphicsPixmapItem *);
};

#endif // MINEMENU_H
