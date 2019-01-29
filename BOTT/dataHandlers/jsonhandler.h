#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <QVector>

class Army;
class Keeper;
class Town;
class Troop;
class View;
class QJsonArray;
class QJsonObject;
class QString;

class JsonHandler
{    
    Keeper * parent;

public:
    JsonHandler(Keeper * parent);

    bool checkExistsOfFieldsAndControlSum(const QJsonObject & mainObj, const QString & field1, const QString & field2, const QString & sum) const;
    bool convertJsonArrayToArmy(const QJsonArray & arr, Army * army) const;
    bool convertJsonArrayToPrototypes(const QJsonArray & arr, Army * army) const;
    bool convertJsonObjectToPriceUpgradesMap(const QJsonObject & obj, View * view) const;
    bool convertJsonObjectToTown(const QJsonObject & obj, Town * town) const;
    bool convertJsonObjectToTroop(const QJsonObject & obj, Troop * troop) const;
    bool loadLastGame(Army * army1, Army * army2, Town * town1, Town * town2, View * view1, View * view2) const;
    bool saveLastGame(const QVector<Troop *> & army1, const QVector<Troop *> & army2,
                      const QVector<Troop *> & prototypes1, const QVector<Troop *> & prototypes2,
                      const Town * town1, const Town * town2,
                      const QMap<QString, int> & upgrades1, const QMap<QString, int> & upgrades2) const;
    bool saveSettings(const QMap<QString, Qt::Key> & settings1, const QMap<QString, Qt::Key> & settings2, const int & musicVolume) const;
    int loadMusicVolume() const;
    QVector<int> * loadSettings(QMap<QString, Qt::Key> & settings, const int & id) const;
    void convertArmyToJsonArray(const QVector<Troop *> & army, QJsonArray & arr) const;
    void convertTownToJsonObject(const Town * town, QJsonObject & obj) const;

private:
    bool loadArmy(const QJsonObject & mainObj, QJsonArray & arr, Army * army, const QString & arm, const QString & controlSum) const;
    bool loadPriceUpgrades(const QJsonObject & mainObj, QJsonObject & obj, View * view, const QString & viewStr, const QString & controlSum) const;
    bool loadPrototypes(const QJsonObject & mainObj, QJsonArray & arr, Army * army, const QString & prototype) const;
    bool loadTown(const QJsonObject & mainObj, QJsonObject & obj, Town * town, const QString & townStr) const;

};

#endif // JSONHANDLER_H
