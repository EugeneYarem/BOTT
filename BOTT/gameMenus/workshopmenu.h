#ifndef WORKSHOPMENU_H
#define WORKSHOPMENU_H

#include "gamemenu.h"

class WorkshopMenu : public GameMenu
{
    Q_OBJECT

public:
    WorkshopMenu();

    // GameMenu interface
    int getPriceOfCurrentItem(const QMap<QString, int> * priceMap, const int & currentItem) const override;
    QVector<int> restoreLastGame(const QMap<QString, int> & restorePriceUpgradeMap) const override;
    void connectWithObject(const QObject * objectForConnect) const override;
    void processSelectAction(const int & currentItem) override;

signals:
    // W - workshop
    void W_ArmorExplored() const;
    void W_ArmorExploredInLastGame() const;
    void W_HauberkExplored() const; // hauberk - кольчуга
    void W_HauberkExploredInLastGame() const;
    void W_MakeArquebus() const;
    void W_MakeArquebusInLastGame() const;
    void W_WeaponLevelUp() const;
    void W_WeaponLevelUpInLastGame() const;

};

#endif // WORKSHOPMENU_H
