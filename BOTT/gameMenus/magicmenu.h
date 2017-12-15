#ifndef MAGICMENU_H
#define MAGICMENU_H

#include "gamemenu.h"

class GameMenuHandler;

class MagicMenu : public GameMenu
{
    Q_OBJECT

public:
    MagicMenu();

signals:
    // M - Magic
    void M_MageHealth();
    void M_MageAttack();

    // GameMenu interface
public:
    void processSelectAction(int );
    void connectWithObject(QObject * );
    int getPriceOfCurrentItem(QMap<QString, int> *, int);
};

#endif // MAGICMENU_H
