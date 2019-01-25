#ifndef HOSPITALMENU_H
#define HOSPITALMENU_H

#include "gamemenu.h"

class HospitalMenu : public GameMenu
{
    Q_OBJECT

public:
    HospitalMenu();

    // GameMenu interface
    int getPriceOfCurrentItem(const QMap<QString, int> * priceMap, const int & currentItem) const override;
    QVector<int> restoreLastGame(const QMap<QString, int> & restorePriceUpgradeMap) const override;
    void connectWithObject(const QObject * objectForConnect) const override;
    void processSelectAction(const int & currentItem) override;

signals:
    // H - Hospital
    void H_DoctorsCountUp() const;
    void H_DoctorsCountUpInLastGame() const;
    void H_EnterQuarantine() const;

};

#endif // HOSPITALMENU_H
