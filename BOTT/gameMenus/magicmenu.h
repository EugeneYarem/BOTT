#ifndef MAGICMENU_H
#define MAGICMENU_H

#include "gamemenu.h"

class MagicMenu : public GameMenu
{
    Q_OBJECT

public:
    MagicMenu();

    // GameMenu interface
    int getPriceOfCurrentItem(const QMap<QString, int> * priceMap, const int & currentItem) const override;
    QVector<int> restoreLastGame(const QMap<QString, int> & restorePriceUpgradeMap) const override;
    void connectWithObject(const QObject * objectForConnect) const override;
    void processSelectAction(const int & currentItem) override;

signals:
    // M - Magic
    void M_MageAttack() const;
    void M_MageHealth() const;

};

#endif // MAGICMENU_H
