#include "constants.h"
#include "jsonhandler.h"
#include "keeper.h"
#include "Military/army.h"
#include "Military/troop.h"
#include "town.h"
#include "view.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>


JsonHandler::JsonHandler(Keeper * parent)
{
    this->parent = parent;
}

bool JsonHandler::checkExistsOfFieldsAndControlSum(const QJsonObject & mainObj, const QString & field1, const QString & field2, const QString & sum) const
{
    return (mainObj[field1] != QJsonValue::Undefined && mainObj[field1].toInt(-1) >= 0 &&
            mainObj[field2] != QJsonValue::Undefined && mainObj[field2].toInt(-1) >= 0 &&
            mainObj[sum] != QJsonValue::Undefined && mainObj[sum].toInt(-1) >= 0);
}

bool JsonHandler::convertJsonArrayToArmy(const QJsonArray & arr, Army * army) const
{
    for(int i = 0; i < arr.count(); i++)
    {
        QJsonObject o = arr.at(i).toObject();
        if(o.isEmpty())
            return false;

        Troop * t = nullptr;

        if(o["type"] != QJsonValue::Undefined && !o["type"].toString().isNull() && !o["type"].toString().isEmpty())
        {
            t = army->createEmptyTroop(o["type"].toString());
            if(t == nullptr)
                return false;
        }
        else return false;

        if(!convertJsonObjectToTroop(o, t))
            return false;

        army->army.append(t);
    }
    return true;
}

bool JsonHandler::convertJsonArrayToPrototypes(const QJsonArray & arr, Army * army) const
{
    for(int i = 0; i < arr.count(); i++)
    {
        QJsonObject o = arr.at(i).toObject();
        if(o.isEmpty())
            return false;

        Troop * t = nullptr;

        if(o["type"] != QJsonValue::Undefined && !o["type"].toString().isNull() && !o["type"].toString().isEmpty())
        {
            t = army->createEmptyTroop(o["type"].toString());

            if(t == nullptr)
                return false;
        }
        else return false;

        if(!convertJsonObjectToTroop(o, t))
            return false;

        army->setNewPrototype(t);
    }
    return true;
}

bool JsonHandler::convertJsonObjectToPriceUpgradesMap(const QJsonObject & obj, View * view) const
{
    const QList<QString> keys = view->getPriceUpgradeMap()->keys();
    view->clearPriceUpgradeMap();

    QJsonObject::const_iterator i = obj.constBegin();
    while(i != obj.constEnd())
    {
        if(!keys.contains(i.key()) || i.value().toInt(-1) < getMinPriceUpgrade())
            return false;
        view->setPriceUpgrade(i.key(), i.value().toInt());
        i++;
    }
    return true;
}

bool JsonHandler::convertJsonObjectToTown(const QJsonObject & obj, Town * town) const
{
    if(obj["health"] != QJsonValue::Undefined && obj["health"].toInt(-1) >= 0 && obj["health"].toInt(-1) <= town::TOWN_HEALTH)
        town->health = obj["health"].toInt();
    else return false;

    if(obj["income"] != QJsonValue::Undefined && obj["income"].toInt(-1) >= town::TOWN_INCOME)
        town->income = obj["income"].toInt();
    else return false;

    if(obj["money"] != QJsonValue::Undefined && obj["money"].toInt(-1) >= 0)
        town->money = obj["money"].toInt();
    else return false;

    if(obj["timer_remainingTime"] != QJsonValue::Undefined && obj["timer_remainingTime"].toInt(-1) >= 0)
        town->incomeTimer_remainingTime = obj["timer_remainingTime"].toInt();
    else return false;

    return true;
}

bool JsonHandler::convertJsonObjectToTroop(const QJsonObject & o, Troop * t) const
{
    qreal x, y, xH, yH;

    if(o["attack"] != QJsonValue::Undefined && o["attack"].toDouble(-1.0) >= 0)
        t->attack = static_cast<int>(o["attack"].toDouble());
    else return false;

    if(o["hp"] != QJsonValue::Undefined && o["hp"].toDouble(-1.0) >= 0)
        t->hp = static_cast<int>(o["hp"].toDouble());
    else return false;

    if(o["animationCounter"] != QJsonValue::Undefined && o["animationCounter"].toInt(-1) >= 1)
        t->animationCounter = o["animationCounter"].toInt();
    else return false;

    if(o["timer_remainingTime"] != QJsonValue::Undefined && o["timer_remainingTime"].toInt(-2) >= -1)
        t->timer_remainingTime = o["timer_remainingTime"].toInt();
    else return false;

    if(o["imgPrefix"] != QJsonValue::Undefined && !o["imgPrefix"].toString().isNull() && !o["imgPrefix"].toString().isEmpty())
        t->imgPrefix = o["imgPrefix"].toString();
    else return false;

    if(o["status"] != QJsonValue::Undefined && o["status"].toInt(-1) >= 0)
        t->status = static_cast<Status>(o["status"].toInt());
    else return false;

    if(o["posX"] != QJsonValue::Undefined && o["posX"].toDouble(-1.0) >= 0)
        x = o["posX"].toDouble();
    else return false;

    if(o["posY"] != QJsonValue::Undefined && o["posY"].toDouble(-1.0) >= 0)
        y = o["posY"].toDouble();
    else return false;

    t->setPos(x, y);

    if(o["hpPosX"] != QJsonValue::Undefined && o["hpPosX"].toDouble(-1.0) >= 0)
        xH = o["hpPosX"].toDouble();
    else return false;

    if(o["hpPosY"] != QJsonValue::Undefined && o["hpPosY"].toDouble(-1.0) >= 0)
        yH = o["hpPosY"].toDouble();
    else return false;

    t->hpText->setPos(xH, yH);

    return true;
}

bool JsonHandler::loadLastGame(Army * army1, Army * army2, Town * town1, Town * town2, View * view1, View * view2) const
{
    QFile file;
    file.setFileName(keeper::LAST_GAME_FILE_PATH);
    try
    {
        if(!file.open(QIODevice::ReadOnly))
            throw 1;
    }
    catch(int )
    {
        file.close();
        return false;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if(jsonDoc.isEmpty() || jsonDoc.isNull() || !jsonDoc.isObject())
        return false;

    QJsonObject mainObj = jsonDoc.object();
    if(mainObj.isEmpty() || mainObj.count() != 27)
        return false;

    if(mainObj["gamerNameP1"] != QJsonValue::Undefined && !mainObj["gamerNameP1"].toString().isNull() && !mainObj["gamerNameP1"].toString().isEmpty())
        parent->gamerNameP1 = mainObj["gamerNameP1"].toString();
    else return false;

    if(mainObj["gamerNameP2"] != QJsonValue::Undefined && !mainObj["gamerNameP2"].toString().isNull() && !mainObj["gamerNameP2"].toString().isEmpty())
        parent->gamerNameP2 = mainObj["gamerNameP2"].toString();
    else return false;

    if(mainObj["gameDuration"] != QJsonValue::Undefined && mainObj["gameDuration"].toInt(-1) >= 0)
        parent->lastGameDuration = mainObj["gameDuration"].toInt();
    else return false;

    if(checkExistsOfFieldsAndControlSum(mainObj, "countOfModificationP1", "countOfModificationP2", "Control sum of countOfModification") &&
       (2 * mainObj["countOfModificationP1"].toInt() + 1) * (mainObj["countOfModificationP2"].toInt() + 2) == mainObj["Control sum of countOfModification"].toInt())
    {
        parent->countOfModificationP1 = mainObj["countOfModificationP1"].toInt();
        parent->countOfModificationP2 = mainObj["countOfModificationP2"].toInt();
    }
    else return false;

    if(checkExistsOfFieldsAndControlSum(mainObj, "countOfUnitsP1", "countOfUnitsP2", "Control sum of countOfUnits") &&
       (2 * mainObj["countOfUnitsP1"].toInt() + 3) * (mainObj["countOfUnitsP2"].toInt() + 4) == mainObj["Control sum of countOfUnits"].toInt())
    {
        parent->countOfUnitsP1 = mainObj["countOfUnitsP1"].toInt();
        parent->countOfUnitsP2 = mainObj["countOfUnitsP2"].toInt();
    }
    else return false;

    if(checkExistsOfFieldsAndControlSum(mainObj, "earnedMoneyP1", "earnedMoneyP2", "Control sum of earnedMoney") &&
       (mainObj["earnedMoneyP1"].toInt() + 3) + (mainObj["earnedMoneyP2"].toInt() + 4) + mainObj["earnedMoneyP2"].toInt() == mainObj["Control sum of earnedMoney"].toInt())
    {
        parent->earnedMoneyP1 = mainObj["earnedMoneyP1"].toInt();
        parent->earnedMoneyP2 = mainObj["earnedMoneyP2"].toInt();
    }
    else return false;

    if(checkExistsOfFieldsAndControlSum(mainObj, "wastedMoneyP1", "wastedMoneyP2", "Control sum of wastedMoney") &&
       (mainObj["wastedMoneyP1"].toInt() + 1) + (mainObj["wastedMoneyP2"].toInt() + 1) + mainObj["wastedMoneyP1"].toInt() == mainObj["Control sum of wastedMoney"].toInt())
    {
        parent->wastedMoneyP1 = mainObj["wastedMoneyP1"].toInt();
        parent->wastedMoneyP2 = mainObj["wastedMoneyP2"].toInt();
    }
    else return false;

    QJsonObject obj;
    if(!loadTown(mainObj, obj, town1, "Town1") || !loadTown(mainObj, obj, town2, "Town2"))
        return false;

    if(loadPriceUpgrades(mainObj, obj, view1, "Upgrades1", "Control sum of Upgrades1") && loadPriceUpgrades(mainObj, obj, view2, "Upgrades2", "Control sum of Upgrades2"))
    {
        view1->modificateFromLastGame();
        view2->modificateFromLastGame();
    }
    else return false;

    QJsonArray arr;
    if(!loadPrototypes(mainObj, arr, army1, "Prototypes1") || !loadPrototypes(mainObj, arr, army2, "Prototypes2"))
        return false;

    if(!loadArmy(mainObj, arr, army1, "Army1", "Control sum of Army1") || !loadArmy(mainObj, arr, army2, "Army2", "Control sum of Army2"))
        return false;

    return true;
}

bool JsonHandler::saveLastGame(const QVector<Troop *> & army1, const QVector<Troop *> & army2,
                               const QVector<Troop *> & prototypes1, const QVector<Troop *> & prototypes2,
                               const Town * town1, const Town * town2,
                               const QMap<QString, int> & upgrades1, const QMap<QString, int> & upgrades2) const
{
    QJsonObject town_1, town_2, up1, up2;
    convertTownToJsonObject(town1, town_1);
    convertTownToJsonObject(town2, town_2);

    QMap<QString, int>::const_iterator i = upgrades1.cbegin();
    while(i != upgrades1.cend())
    {
        up1.insert(i.key(), i.value());
        i++;
    }

    i = upgrades2.cbegin();
    while(i != upgrades2.cend())
    {
        up2.insert(i.key(), i.value());
        i++;
    }

    QJsonArray arm1, arm2;
    convertArmyToJsonArray(army1, arm1);
    convertArmyToJsonArray(army2, arm2);

    if(prototypes1.size() != army::TROOP_TYPES_COUNT || prototypes2.size() != army::TROOP_TYPES_COUNT)
        return false;

    QJsonArray prot1, prot2;
    convertArmyToJsonArray(prototypes1, prot1);
    convertArmyToJsonArray(prototypes2, prot2);

    QJsonObject mainObj;
    mainObj.insert("Town1", town_1);
    mainObj.insert("Town2", town_2);
    mainObj.insert("Army1", arm1);
    mainObj.insert("Control sum of Army1", (arm1.size() * arm1.size() + 10) * 4);
    mainObj.insert("Army2", arm2);
    mainObj.insert("Control sum of Army2", (arm2.size() * arm2.size() + 10) * 4);
    mainObj.insert("Prototypes1", prot1);
    mainObj.insert("Prototypes2", prot2);
    mainObj.insert("Upgrades1", up1);
    mainObj.insert("Control sum of Upgrades1", up1.size() * up1.size() * 3);
    mainObj.insert("Upgrades2", up2);
    mainObj.insert("Control sum of Upgrades2", up2.size() * up2.size() * 3);
    mainObj.insert("gamerNameP1", parent->gamerNameP1);
    mainObj.insert("gamerNameP2", parent->gamerNameP2);
    mainObj.insert("gameDuration", parent->gameDuration.msecsTo(QTime::currentTime()));
    mainObj.insert("countOfModificationP1", parent->countOfModificationP1);
    mainObj.insert("countOfModificationP2", parent->countOfModificationP2);
    mainObj.insert("Control sum of countOfModification", (2 * parent->countOfModificationP1 + 1) * (parent->countOfModificationP2 + 2));
    mainObj.insert("countOfUnitsP1", parent->countOfUnitsP1);
    mainObj.insert("countOfUnitsP2", parent->countOfUnitsP2);
    mainObj.insert("Control sum of countOfUnits", (2 * parent->countOfUnitsP1 + 3) * (parent->countOfUnitsP2 + 4));
    mainObj.insert("earnedMoneyP1", parent->earnedMoneyP1);
    mainObj.insert("earnedMoneyP2", parent->earnedMoneyP2);
    mainObj.insert("Control sum of earnedMoney", (parent->earnedMoneyP1 + 3) + (parent->earnedMoneyP2 + 4) + parent->earnedMoneyP2);
    mainObj.insert("wastedMoneyP1", parent->wastedMoneyP1);
    mainObj.insert("wastedMoneyP2", parent->wastedMoneyP2);
    mainObj.insert("Control sum of wastedMoney", (parent->wastedMoneyP1 + 1) + (parent->wastedMoneyP2 + 1) + parent->wastedMoneyP1);

    QJsonDocument doc;
    doc.setObject(mainObj);

    if(!QFile::exists(keeper::DATA_DIR_PATH))
        QDir().mkdir(keeper::DATA_DIR_PATH);

    QFile file;
    file.setFileName(keeper::LAST_GAME_FILE_PATH);
    try
    {
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            throw -1;
    }
    catch(int )
    {
        file.close();
        return false;
    }

    file.write(doc.toJson());
    file.close();
    return true;
}

bool JsonHandler::saveSettings(const QMap<QString, Qt::Key> & settings1, const QMap<QString, Qt::Key> & settings2, const int & musicVolume) const
{
    if(settings1.isEmpty() || settings2.isEmpty())
        return false;

    QJsonObject player1, player2;
    player1.insert("menu", settings1.value("menu"));
    player1.insert("menu up", settings1.value("menu up"));
    player1.insert("menu down", settings1.value("menu down"));
    player1.insert("menu select", settings1.value("menu select"));
    player1.insert("exit from menu", settings1.value("exit from menu"));
    player1.insert("create soldier", settings1.value("create soldier"));
    player1.insert("create archer", settings1.value("create archer"));
    player1.insert("create rider", settings1.value("create rider"));
    player1.insert("create wizard", settings1.value("create wizard"));

    player2.insert("menu", settings2.value("menu"));
    player2.insert("menu up", settings2.value("menu up"));
    player2.insert("menu down", settings2.value("menu down"));
    player2.insert("menu select", settings2.value("menu select"));
    player2.insert("exit from menu", settings2.value("exit from menu"));
    player2.insert("create soldier", settings2.value("create soldier"));
    player2.insert("create archer", settings2.value("create archer"));
    player2.insert("create rider", settings2.value("create rider"));
    player2.insert("create wizard", settings2.value("create wizard"));

    QJsonArray array;
    array.insert(0, player1);
    array.insert(1, player2);
    array.insert(2, musicVolume);

    QJsonDocument jsonDoc;
    jsonDoc.setArray(array);

    if(!QFile::exists(keeper::DATA_DIR_PATH))
        QDir().mkdir(keeper::DATA_DIR_PATH);

    QFile file;
    file.setFileName(keeper::SETTING_FILE_PATH);
    try
    {
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            throw -1;
    }
    catch(int )
    {
        file.close();
        return false;
    }

    file.write(jsonDoc.toJson());
    file.close();
    return true;
}

int JsonHandler::loadMusicVolume() const
{
    QFile file;
    file.setFileName(keeper::SETTING_FILE_PATH);
    try
    {
        if(!file.open(QIODevice::ReadOnly))
            throw 1;
    }
    catch(int )
    {
        file.close();
        return -1; // ошибка: файла настроек нет
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if(jsonDoc.isEmpty() || jsonDoc.isNull() || !jsonDoc.isArray())
    {
        return -1; // ошибка: нет файла
    }

    return jsonDoc.array().at(2).toInt(-1);
}

QVector<int> * JsonHandler::loadSettings(QMap<QString, Qt::Key> & settings, const int & id) const
{
    QFile file;
    file.setFileName(keeper::SETTING_FILE_PATH);
    try
    {
        if(!file.open(QIODevice::ReadOnly))
            throw 1;
    }
    catch(int )
    {
        file.close();
        return new QVector<int>(QVector<int>::fromStdVector(general::ERRORS_READING_SETTINGS)); // ошибка: нет файла
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if(jsonDoc.isEmpty() || jsonDoc.isNull() || !jsonDoc.isArray())
    {
        return new QVector<int>(QVector<int>::fromStdVector(general::ERRORS_READING_SETTINGS)); // ошибка: нет файла
    }

    QVector<int> *errors = new QVector<int>;

    if(jsonDoc.array().at( id ).toObject().value("menu") == QJsonValue::Undefined)
        errors->push_back(0);
    else settings.insert("menu", static_cast<Qt::Key>( jsonDoc.array().at( id ).toObject().value("menu").toInt() ));

    if(jsonDoc.array().at( id ).toObject().value("menu up") == QJsonValue::Undefined)
        errors->push_back(1);
    else settings.insert("menu up", static_cast<Qt::Key>( jsonDoc.array().at( id ).toObject().value("menu up").toInt() ));

    if(jsonDoc.array().at( id ).toObject().value("menu down") == QJsonValue::Undefined)
        errors->push_back(2);
    else settings.insert("menu down", static_cast<Qt::Key>( jsonDoc.array().at( id ).toObject().value("menu down").toInt() ));

    if(jsonDoc.array().at( id ).toObject().value("menu select") == QJsonValue::Undefined)
        errors->push_back(3);
    else settings.insert("menu select", static_cast<Qt::Key>( jsonDoc.array().at( id ).toObject().value("menu select").toInt() ));

    if(jsonDoc.array().at( id ).toObject().value("exit from menu") == QJsonValue::Undefined)
        errors->push_back(4);
    else settings.insert("exit from menu", static_cast<Qt::Key>( jsonDoc.array().at( id ).toObject().value("exit from menu").toInt() ));

    if(jsonDoc.array().at( id ).toObject().value("create soldier") == QJsonValue::Undefined)
        errors->push_back(5);
    else settings.insert("create soldier", static_cast<Qt::Key>( jsonDoc.array().at( id ).toObject().value("create soldier").toInt() ));

    if(jsonDoc.array().at( id ).toObject().value("create archer") == QJsonValue::Undefined)
        errors->push_back(6);
    else settings.insert("create archer", static_cast<Qt::Key>( jsonDoc.array().at( id ).toObject().value("create archer").toInt() ));

    if(jsonDoc.array().at( id ).toObject().value("create rider") == QJsonValue::Undefined)
        errors->push_back(7);
    else settings.insert("create rider", static_cast<Qt::Key>( jsonDoc.array().at( id ).toObject().value("create rider").toInt() ));

    if(jsonDoc.array().at( id ).toObject().value("create wizard") == QJsonValue::Undefined)
        errors->push_back(8);
    else settings.insert("create wizard", static_cast<Qt::Key>( jsonDoc.array().at( id ).toObject().value("create wizard").toInt() ));

    return errors;
}

void JsonHandler::convertArmyToJsonArray(const QVector<Troop *> & army, QJsonArray & arr) const
{
    foreach(const Troop * i, army)
    {
        QJsonObject t;
        t.insert("attack", i->attack);
        t.insert("hp", i->hp);
        t.insert("animationCounter", i->animationCounter);
        t.insert("timer_remainingTime", i->timer_remainingTime);
        t.insert("imgPrefix", i->imgPrefix);
        t.insert("type", i->type);
        t.insert("status", i->status);
        t.insert("posX", i->x());
        t.insert("posY", i->y());
        t.insert("hpPosX", i->hpText->x());
        t.insert("hpPosY", i->hpText->y());
        arr.append(t);
    }
}

void JsonHandler::convertTownToJsonObject(const Town * town, QJsonObject & obj) const
{
    obj.insert("health", town->health);
    obj.insert("income", town->income);
    obj.insert("money", town->money);
    obj.insert("timer_remainingTime", town->incomeTimer_remainingTime);
}

bool JsonHandler::loadArmy(const QJsonObject & mainObj, QJsonArray & arr, Army * army, const QString & arm, const QString & controlSum) const
{
    if(mainObj[arm] != QJsonValue::Undefined && mainObj[arm].isArray() && mainObj[controlSum] != QJsonValue::Undefined && mainObj[controlSum].toInt(-1) >= 0)
    {
        arr = mainObj[arm].toArray();
        if(arr.size() == 0 && mainObj[controlSum].toInt() == 40)
            return true;
        if(arr.size() == ((mainObj[controlSum].toInt() / 4 - 10) / arr.size()) && arr.size() <= army::MAX_ARMY_SIZE)
            return convertJsonArrayToArmy(arr, army);
        else return false;
    }
    else return false;
}

bool JsonHandler::loadPriceUpgrades(const QJsonObject & mainObj, QJsonObject & obj, View * view, const QString & viewStr, const QString & controlSum) const
{
    if(mainObj[viewStr] != QJsonValue::Undefined && mainObj[viewStr].isObject() && mainObj[controlSum] != QJsonValue::Undefined && mainObj[controlSum].toInt(-1) >= 0)
    {
        obj = mainObj[viewStr].toObject();
        if(obj.count() == (mainObj[controlSum].toInt() / obj.size() / 3) && obj.count() <= general::UPGRADES_COUNT)
            return convertJsonObjectToPriceUpgradesMap(obj, view);
        else return false;
    }
    else return false;
}

bool JsonHandler::loadPrototypes(const QJsonObject & mainObj, QJsonArray & arr, Army * army, const QString & prototype) const
{
    if(mainObj[prototype] != QJsonValue::Undefined && mainObj[prototype].isArray())
    {
        arr = mainObj[prototype].toArray();
        if(arr.size() == army::TROOP_TYPES_COUNT)
            return convertJsonArrayToPrototypes(arr, army);
        else return false;
    }
    else return false;
}

bool JsonHandler::loadTown(const QJsonObject  &mainObj, QJsonObject & obj, Town * town, const QString & townStr) const
{
    if(mainObj[townStr] != QJsonValue::Undefined && mainObj[townStr].isObject())
    {
        obj = mainObj[townStr].toObject();
        if(obj.count() == 4)
            return convertJsonObjectToTown(obj, town);
        else return false;
    }
    else return false;
}
