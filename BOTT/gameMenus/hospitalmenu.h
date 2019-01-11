#ifndef HOSPITALMENU_H
#define HOSPITALMENU_H

#include "gamemenu.h"

class HospitalMenu : public GameMenu
{
    Q_OBJECT

public:
    HospitalMenu();

    // GameMenu interface
    int getPriceOfCurrentItem(QMap<QString, int> * priceMap, int currentItem) override;
    void connectWithObject(QObject * objectForConnect) override;
    void processSelectAction(int currentItem) override;

signals:
    // H - Hospital
    void H_DoctorsCountUp();
    void H_EnterQuarantine();

};

#endif // HOSPITALMENU_H
