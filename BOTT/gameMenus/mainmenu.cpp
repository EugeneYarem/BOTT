#include "gamemenuhandler.h"
#include "mainmenu.h"
#include "town.h"
#include "view.h"
#include "widget.h"


MainMenu::MainMenu()
{
    nameOfMenu = new QGraphicsPixmapItem();
    nameOfMenu->setPixmap(QPixmap(":images/images/Main_Menu/mMain.png"));

    QGraphicsPixmapItem * m1 = new QGraphicsPixmapItem(),
                        * m2 = new QGraphicsPixmapItem(),
                        * m3 = new QGraphicsPixmapItem(),
                        * m4 = new QGraphicsPixmapItem(),
                        * m5 = new QGraphicsPixmapItem();

    m1->setPixmap(QPixmap(":images/images/Main_Menu/mMag.png"));
    m2->setPixmap(QPixmap(":images/images/Main_Menu/mW.png"));
    m3->setPixmap(QPixmap(":images/images/Main_Menu/mH.png"));
    m4->setPixmap(QPixmap(":images/images/Main_Menu/mM.png"));
    m5->setPixmap(QPixmap(":images/images/Main_Menu/mC.png"));
    menuItems.push_back(m1);
    menuItems.push_back(m2);
    menuItems.push_back(m3);
    menuItems.push_back(m4);
    menuItems.push_back(m5);

    eventTypeForDispMess = QEvent::registerEventType();
}

void MainMenu::processSelectAction(const int & currentItem)
{
    if(currentItem == -1)
        return;

    int cI = currentItem; // Не удалять, т.к. в parent->hideCurrentOpenMenu() поле currentItem обнуляется, и меняеются здесь, т. к. передается по ссылке

    parent->hideCurrentOpenMenu();

    if(cI == 0)
        parent->showMagicMenu();
    if(cI == 1)
        parent->showWorkshopMenu();
    if(cI == 2)
        parent->showHospitalMenu();
    if(cI == 3)
        parent->showMineMenu();
    if(cI == 4)
    {
        parent->showMainMenu();
        emit requiredShowMes("Вы точно желаете сдаться?", this, eventTypeForDispMess);
    }
}

void MainMenu::processExitAction() const
{
    emit parent->closeMenu();
}

void MainMenu::connectWithObject(const QObject * objectForConnect) const
{
    if(typeid(*objectForConnect) == typeid(Widget))
        connect(this, SIGNAL(requiredShowMes(QString , QObject * , int )), dynamic_cast<const Widget *>(objectForConnect), SLOT(showMessage(QString , QObject *, int )));
}

void MainMenu::deleteMenuItem(const int & item)
{
    parent->scene()->removeItem(menuItems.at(item));
    delete menuItems.at(item);
}

int MainMenu::getPriceOfCurrentItem(const QMap<QString, int> * , const int & ) const
{
    return -1;
}

QVector<int> MainMenu::restoreLastGame(const QMap<QString, int> & ) const
{}

bool MainMenu::event(QEvent * ev)
{
    if(ev->type() == eventTypeForDispMess)
    {
        emit parent->getParentView()->getTown()->loose();
        return true;
    }
    return GameMenu::event(ev);
}
