#ifndef MINEMENU_H
#define MINEMENU_H

#include "gamemenu.h"

class MineMenu : public GameMenu
{
    Q_OBJECT

public:
    MineMenu();

    // GameMenu interface
    int getPriceOfCurrentItem(const QMap<QString, int> * priceMap, const int & currentItem) const override;
    QVector<int> restoreLastGame(const QMap<QString, int> & restorePriceUpgradeMap) const override;
    void connectWithObject(const QObject * objectForConnect) const override;
    void processSelectAction(const int & currentItem) override;

signals:
    // M - Mine
    void M_LevelUp() const;

};

#endif // MINEMENU_H
