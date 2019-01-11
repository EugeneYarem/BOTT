#ifndef MAINMENU_H
#define MAINMENU_H

#include "gamemenu.h"

class MainMenu : public GameMenu
{
    Q_OBJECT

    int eventTypeForDispMess;

public:
    MainMenu();

    bool event(QEvent * event) override;
    int getPriceOfCurrentItem(QMap<QString, int> * priceMap, int currentItem) override;
    void connectWithObject(QObject * objectForConnect) override;
    void deleteMenuItem(int item);
    void processExitAction() override;
    void processSelectAction(int currentItem) override;

signals:
    void requiredShowMes(QString text, QObject * sender, int eventType);

};

#endif // MAINMENU_H
