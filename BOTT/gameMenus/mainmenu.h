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
    int getPriceOfCurrentItem(const QMap<QString, int> * priceMap, const int & currentItem) const override;
    QVector<int> restoreLastGame(const QMap<QString, int> & restorePriceUpgradeMap) const override;
    void connectWithObject(const QObject * objectForConnect) const override;
    void deleteMenuItem(const int & item);
    void processExitAction() const override;
    void processSelectAction(const int & currentItem) override;

signals:
    void requiredShowMes(QString text, QObject * sender, int eventType) const;

};

#endif // MAINMENU_H
