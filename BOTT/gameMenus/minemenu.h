#ifndef MINEMENU_H
#define MINEMENU_H

#include "gamemenu.h"

class MineMenu : public GameMenu
{
    Q_OBJECT

public:
    MineMenu();

    // GameMenu interface
    int getPriceOfCurrentItem(QMap<QString, int> * priceMap, int currentItem) override;
    void connectWithObject(QObject * objectForConnect) override;
    void processSelectAction(int currentItem) override;

signals:
    // M - Mine
    void M_LevelUp();

};

#endif // MINEMENU_H
