#ifndef MAGICMENU_H
#define MAGICMENU_H

#include "gamemenu.h"

class MagicMenu : public GameMenu
{
    Q_OBJECT

public:
    MagicMenu();

    // GameMenu interface
    int getPriceOfCurrentItem(QMap<QString, int> * priceMap, int currentItem) override;
    void connectWithObject(QObject * objectForConnect) override;
    void processSelectAction(int currentItem) override;

signals:
    // M - Magic
    void M_MageAttack();
    void M_MageHealth();

};

#endif // MAGICMENU_H
