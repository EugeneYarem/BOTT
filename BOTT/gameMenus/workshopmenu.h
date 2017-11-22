#ifndef WORKSHOPMENU_H
#define WORKSHOPMENU_H

#include "gamemenu.h"

class GameMenuHandler;

class WorkshopMenu : public GameMenu
{
    Q_OBJECT

public:
    WorkshopMenu();

signals:
    // W - workshop
    void W_MakeGun();
    void W_MakeArquebus();
    void W_HauberkExplored(); // hauberk - кольчуга
    void W_ArmorExplored();
    void W_WeaponLevelUp();

    // GameMenu interface
public:
    void processSelectAction(int );
    void connectWithObject(QObject * );
};

#endif // WORKSHOPMENU_H
