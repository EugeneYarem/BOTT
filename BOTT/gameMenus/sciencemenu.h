#ifndef SCIENCEMENU_H
#define SCIENCEMENU_H

#include "gamemenu.h"

class GameMenuHandler;

class ScienceMenu : public GameMenu
{
    Q_OBJECT

public:
    ScienceMenu();

signals:
    // SC - Science Center
    void SC_MageHealth();
    void SC_MageAttack();

    // GameMenu interface
public:
    void processSelectAction(int );
    void connectWithObject(QObject * );
    int getPriceOfCurrentItem(QMap<QString, int> *, int);
};

#endif // SCIENCEMENU_H
