#include "gamemenuhandler.h"
#include "hospitalmenu.h"
#include "Military/army.h"


HospitalMenu::HospitalMenu()
{
    nameOfMenu = new QGraphicsPixmapItem();
    nameOfMenu->setPixmap(QPixmap(":images/images/Hospital_Menu/mHospital.png"));

    QGraphicsPixmapItem * m1 = new QGraphicsPixmapItem(),
                        * m2 = new QGraphicsPixmapItem();

    m1->setPixmap(QPixmap(":images/images/Hospital_Menu/h1.png"));
    m2->setPixmap(QPixmap(":images/images/Hospital_Menu/h2.png"));
    menuItems.push_back(m1);
    menuItems.push_back(m2);
}

void HospitalMenu::processSelectAction(const int & currentItem)
{
    if(currentItem == -1)
        return;
    if(currentItem == 0)
        emit H_EnterQuarantine();
    if(currentItem == 1)
        emit H_DoctorsCountUp();
}

void HospitalMenu::connectWithObject(const QObject * objectForConnect) const
{
    if(typeid(*objectForConnect) == typeid(Army))
    {
        connect(this, &HospitalMenu::H_EnterQuarantine, dynamic_cast<const Army *>(objectForConnect), &Army::improveQuarantine);
        connect(this, &HospitalMenu::H_DoctorsCountUp, dynamic_cast<const Army *>(objectForConnect), &Army::improveDoctors);
        connect(this, &HospitalMenu::H_DoctorsCountUpInLastGame, dynamic_cast<const Army *>(objectForConnect), &Army::setDoctorsImprove);
    }
}

int HospitalMenu::getPriceOfCurrentItem(const QMap<QString, int> * map, const int & currentItem) const
{
    if(currentItem == -1)
        return 0;
    if(currentItem == 0)
        return map->value("Quarantine");
    if(currentItem == 1)
        return map->value("Doctors");
}

QVector<int> HospitalMenu::restoreLastGame(const QMap<QString, int> & rpum) const
{
    QVector<int> vec;
    if(!rpum.contains("Quarantine"))
        vec.append(0);
    if(!rpum.contains("Doctors"))
    {
        vec.append(1);
        emit H_DoctorsCountUpInLastGame();
    }

    return vec;
}
