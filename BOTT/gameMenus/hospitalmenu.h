#ifndef HOSPITALMENU_H
#define HOSPITALMENU_H

#include "gamemenu.h"

class GameMenuHandler;

class HospitalMenu : public GameMenu
{
    Q_OBJECT

public:
    HospitalMenu();

signals:
    // H - Hospital
    void H_EnterQuarantine();
    void H_DoctorsCountUp();

    // GameMenu interface
public:
    void processSelectAction(int );
    void connectWithObject(QObject * );
    int getPriceOfCurrentItem(QMap<QString, int> *, int);
};

#endif // HOSPITALMENU_H
