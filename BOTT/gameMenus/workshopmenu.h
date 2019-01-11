#ifndef WORKSHOPMENU_H
#define WORKSHOPMENU_H

#include "gamemenu.h"

class WorkshopMenu : public GameMenu
{
    Q_OBJECT

public:
    WorkshopMenu();

    // GameMenu interface
    int getPriceOfCurrentItem(QMap<QString, int> * priceMap, int currentItem) override;
    void connectWithObject(QObject * objectForConnect) override;
    void processSelectAction(int currentItem) override;

signals:
    // W - workshop
    void W_ArmorExplored();
    void W_HauberkExplored(); // hauberk - кольчуга
    void W_MakeArquebus();
    void W_WeaponLevelUp();

};

#endif // WORKSHOPMENU_H
