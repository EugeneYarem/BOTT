#include "constants.h"
#include "keeper.h"
#include "Military/army.h"
#include "Military/troop.h"
#include "town.h"
#include "view.h"
#include "widget.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QSqlQuery>
#include <QTimer>
#include <QVariant>
#include <QVector>


Keeper::Keeper(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    connectDB();

    refreshStatistics();
    countOfGamesRecords = -1;
}

Keeper::~Keeper()
{
    db.close();
}

bool Keeper::checkExistsOfFieldsAndControlSum(const QJsonObject & mainObj, const QString & field1, const QString & field2, const QString & sum) const
{
    return (mainObj[field1] != QJsonValue::Undefined && mainObj[field1].toInt(-1) >= 0 &&
       mainObj[field2] != QJsonValue::Undefined && mainObj[field2].toInt(-1) >= 0 &&
       mainObj[sum] != QJsonValue::Undefined && mainObj[sum].toInt(-1) >= 0);
}

bool Keeper::loadLastGame(Army * army1, Army * army2,
                          Town * town1, Town * town2,
                          View * view1, View * view2)
{
    QFile file;
    file.setFileName(LAST_GAME_FILE_PATH);
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
        gamerNameP1 = mainObj["gamerNameP1"].toString();
    else return false;

    if(mainObj["gamerNameP2"] != QJsonValue::Undefined && !mainObj["gamerNameP2"].toString().isNull() && !mainObj["gamerNameP2"].toString().isEmpty())
        gamerNameP2 = mainObj["gamerNameP2"].toString();
    else return false;

    if(mainObj["gameDuration"] != QJsonValue::Undefined && mainObj["gameDuration"].toInt(-1) >= 0)
        lastGameDuration = mainObj["gameDuration"].toInt();
    else return false;

    if(checkExistsOfFieldsAndControlSum(mainObj, "countOfModificationP1", "countOfModificationP2", "Control sum of countOfModification") &&
       (2 * mainObj["countOfModificationP1"].toInt() + 1) * (mainObj["countOfModificationP2"].toInt() + 2) == mainObj["Control sum of countOfModification"].toInt())
    {
        countOfModificationP1 = mainObj["countOfModificationP1"].toInt();
        countOfModificationP2 = mainObj["countOfModificationP2"].toInt();
    }
    else return false;

    if(checkExistsOfFieldsAndControlSum(mainObj, "countOfUnitsP1", "countOfUnitsP2", "Control sum of countOfUnits") &&
       (2 * mainObj["countOfUnitsP1"].toInt() + 3) * (mainObj["countOfUnitsP2"].toInt() + 4) == mainObj["Control sum of countOfUnits"].toInt())
    {
        countOfUnitsP1 = mainObj["countOfUnitsP1"].toInt();
        countOfUnitsP2 = mainObj["countOfUnitsP2"].toInt();
    }
    else return false;

    if(checkExistsOfFieldsAndControlSum(mainObj, "earnedMoneyP1", "earnedMoneyP2", "Control sum of earnedMoney") &&
       (mainObj["earnedMoneyP1"].toInt() + 3) + (mainObj["earnedMoneyP2"].toInt() + 4) + mainObj["earnedMoneyP2"].toInt() == mainObj["Control sum of earnedMoney"].toInt())
    {
        earnedMoneyP1 = mainObj["earnedMoneyP1"].toInt();
        earnedMoneyP2 = mainObj["earnedMoneyP2"].toInt();
    }
    else return false;

    if(checkExistsOfFieldsAndControlSum(mainObj, "wastedMoneyP1", "wastedMoneyP2", "Control sum of wastedMoney") &&
       (mainObj["wastedMoneyP1"].toInt() + 1) + (mainObj["wastedMoneyP2"].toInt() + 1) + mainObj["wastedMoneyP1"].toInt() == mainObj["Control sum of wastedMoney"].toInt())
    {
        wastedMoneyP1 = mainObj["wastedMoneyP1"].toInt();
        wastedMoneyP2 = mainObj["wastedMoneyP2"].toInt();
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

bool Keeper::saveLastGame(const QVector<Troop *> & army1, const QVector<Troop *> & army2,
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

    if(prototypes1.size() != TROOP_TYPES_COUNT || prototypes2.size() != TROOP_TYPES_COUNT)
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
    mainObj.insert("gamerNameP1", gamerNameP1);
    mainObj.insert("gamerNameP2", gamerNameP2);
    mainObj.insert("gameDuration", gameDuration.msecsTo(QTime::currentTime()));
    mainObj.insert("countOfModificationP1", countOfModificationP1);
    mainObj.insert("countOfModificationP2", countOfModificationP2);
    mainObj.insert("Control sum of countOfModification", (2 * countOfModificationP1 + 1) * (countOfModificationP2 + 2));
    mainObj.insert("countOfUnitsP1", countOfUnitsP1);
    mainObj.insert("countOfUnitsP2", countOfUnitsP2);
    mainObj.insert("Control sum of countOfUnits", (2 * countOfUnitsP1 + 3) * (countOfUnitsP2 + 4));
    mainObj.insert("earnedMoneyP1", earnedMoneyP1);
    mainObj.insert("earnedMoneyP2", earnedMoneyP2);
    mainObj.insert("Control sum of earnedMoney", (earnedMoneyP1 + 3) + (earnedMoneyP2 + 4) + earnedMoneyP2);
    mainObj.insert("wastedMoneyP1", wastedMoneyP1);
    mainObj.insert("wastedMoneyP2", wastedMoneyP2);
    mainObj.insert("Control sum of wastedMoney", (wastedMoneyP1 + 1) + (wastedMoneyP2 + 1) + wastedMoneyP1);

    QJsonDocument doc;
    doc.setObject(mainObj);

    if(!QFile::exists(DATA_DIR_PATH))
        QDir().mkdir(DATA_DIR_PATH);

    QFile file;
    file.setFileName(LAST_GAME_FILE_PATH);
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

QVector<int> * Keeper::loadSettings(QMap<QString, Qt::Key> & settings, const int & id) const
{
    QFile file;
    file.setFileName(SETTING_FILE_PATH);
    try
    {
        if(!file.open(QIODevice::ReadOnly))
            throw 1;
    }
    catch(int )
    {
        file.close();
        return new QVector<int>(QVector<int>::fromStdVector(ERRORS_READING_SETTINGS)); // ошибка: нет файла
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if(jsonDoc.isEmpty() || jsonDoc.isNull() || !jsonDoc.isArray())
    {
        return new QVector<int>(QVector<int>::fromStdVector(ERRORS_READING_SETTINGS)); // ошибка: нет файла
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

bool Keeper::saveSettings(const QMap<QString, Qt::Key> & settings1, const QMap<QString, Qt::Key> & settings2, const int & musicVolume) const
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

    if(!QFile::exists(DATA_DIR_PATH))
        QDir().mkdir(DATA_DIR_PATH);

    QFile file;
    file.setFileName(SETTING_FILE_PATH);
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

bool Keeper::saveStatistics() const
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO games (time, gamer1_id, gamer2_id) VALUES (:time, :gamer1_id, :gamer2_id)");
    query.bindValue(":time", gameDuration.msecsTo(QTime::currentTime()));
    if(countOfGamesRecords == 0)
    {
        query.bindValue(":gamer1_id", 1);
        query.bindValue(":gamer2_id", 2);
    }
    else
    {
        query.bindValue(":gamer1_id", countOfGamesRecords * 2 + 1);
        query.bindValue(":gamer2_id", countOfGamesRecords * 2 + 2);
    }

    try
    {
        if(!query.exec())
            throw 1;
    }
    catch(int )
    {
        emit requiredShowMes("Не удалось сохранить результат игры. Потеряно соединение с БД.");
        return false;
    }

    QSqlQuery player1(db);
    player1.prepare("INSERT INTO gamers (name, result, earnedMoney, wastedMoney, countOfUnits, countOdMods) VALUES (:name, :result, :earnedMoney, :wastedMoney, :countOfUnits, :countOdMods)");
    QSqlQuery player2(db);
    player2.prepare("INSERT INTO gamers (name, result, earnedMoney, wastedMoney, countOfUnits, countOdMods) VALUES (:name, :result, :earnedMoney, :wastedMoney, :countOfUnits, :countOdMods)");

    if(!winner.isEmpty())
    {
        if(winner == gamerNameP1)
        {
            player1.bindValue(":result", QString("Выиграл"));
            player2.bindValue(":result", QString("Проиграл"));
        }
        else
        {
            player2.bindValue(":result", QString("Выиграл"));
            player1.bindValue(":result", QString("Проиграл"));
        }
    }
    else
    {
        player1.bindValue(":result", QString("Ничья"));
        player2.bindValue(":result", QString("Ничья"));
    }

    player1.bindValue(":name", gamerNameP1);
    player1.bindValue(":earnedMoney", earnedMoneyP1);
    player1.bindValue(":wastedMoney", wastedMoneyP1);
    player1.bindValue(":countOfUnits", countOfUnitsP1);
    player1.bindValue(":countOdMods", countOfModificationP1);
    player2.bindValue(":name", gamerNameP2);
    player2.bindValue(":earnedMoney", earnedMoneyP2);
    player2.bindValue(":wastedMoney", wastedMoneyP2);
    player2.bindValue(":countOfUnits", countOfUnitsP2);
    player2.bindValue(":countOdMods", countOfModificationP2);

    try
    {
        if(!player1.exec() || !player2.exec())
            throw 1;
    }
    catch(int )
    {
        emit requiredShowMes("Не удалось сохранить результат игры. Потеряно соединение с БД.");
        return false;
    }

    return true;
}

QSqlQuery & Keeper::getGamesRecords() const
{
    static QSqlQuery query;

    try
    {
        if(!query.exec("SELECT time, gamer1_id, gamer2_id FROM games ORDER BY gamer1_id DESC"))
            throw 1;
    }
    catch(int )
    {
        emit requiredShowMes("Не удалось получить статистику. Потеряно соединение с БД.");
        return query;
    }
    return query;
}

QSqlQuery & Keeper::getRecordAboutGamer(const int & id) const
{
    static QSqlQuery player;
    player.prepare("SELECT name, result, earnedMoney, wastedMoney, countOfUnits, countOdMods FROM gamers WHERE id = :id");
    player.bindValue(":id", id);

    try
    {
        if(!player.exec())
            throw 1;
    }
    catch(int )
    {
        emit requiredShowMes("Не удалось получить статистику. Потеряно соединение с БД.");
        return player;
    }
    return player;
}

int Keeper::getCountOfGamesRecords(const bool & refreshCount)
{
    if(countOfGamesRecords == -1 || refreshCount)
    {
        QSqlQuery query = getGamesRecords();

        int res = 0;
        while (query.next())
            res++;

        countOfGamesRecords = res;
    }
    return countOfGamesRecords;
}

int Keeper::loadMusicVolume() const
{
    QFile file;
    file.setFileName(SETTING_FILE_PATH);
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

void Keeper::refreshStatistics()
{
    gameDuration = QTime::currentTime();
    earnedMoneyP1 = 0;
    earnedMoneyP2 = 0;
    countOfModificationP1 = 0;
    countOfModificationP2 = 0;
    countOfUnitsP1 = 0;
    countOfUnitsP2 = 0;
    lastGameDuration = -1;
    wastedMoneyP1 = 0;
    wastedMoneyP2 = 0;
    winner = "";
}

void Keeper::removeLastGameFile() const
{
    QFile(LAST_GAME_FILE_PATH).remove();
}

void Keeper::setGamersNames(const QString & gamer1, const QString & gamer2)
{
    gamerNameP1 = gamer1;
    gamerNameP2 = gamer2;
}

const QString & Keeper::getWinner() const
{
    return winner;
}

bool Keeper::connectDB()
{
    db.setDatabaseName(DATABASE_FILE_PATH);

    if(!QFile::exists(DATA_DIR_PATH))
        QDir().mkdir(DATA_DIR_PATH);

    try
    {
        if(!db.open())
            throw 1;
    }
    catch(int )
    {
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT * FROM games");

    try
    {
        if(!query.exec())
            throw 1;
    }
    catch(int )
    {
        query.exec("CREATE TABLE games (id INTEGER PRIMARY KEY UNIQUE, time INT, gamer1_id INT, gamer2_id INT)");
        query.exec("CREATE TABLE gamers (id INTEGER PRIMARY KEY UNIQUE, name STRING, result STRING, earnedMoney INT, wastedMoney INT, countOfUnits INT, countOdMods INT)");
    }

    return true;
}

bool Keeper::loadArmy(const QJsonObject & mainObj, QJsonArray & arr, Army * army, const QString & arm, const QString & controlSum) const
{
    if(mainObj[arm] != QJsonValue::Undefined && mainObj[arm].isArray() && mainObj[controlSum] != QJsonValue::Undefined && mainObj[controlSum].toInt(-1) >= 0)
    {
        arr = mainObj[arm].toArray();
        if(arr.size() == 0 && mainObj[controlSum].toInt() == 40)
            return true;
        if(arr.size() == ((mainObj[controlSum].toInt() / 4 - 10) / arr.size()))
            return convertJsonArrayToArmy(arr, army);
        else return false;
    }
    else return false;
}

bool Keeper::loadPriceUpgrades(const QJsonObject & mainObj, QJsonObject & obj, View * view, const QString & viewStr, const QString & controlSum) const
{
    if(mainObj[viewStr] != QJsonValue::Undefined && mainObj[viewStr].isObject() && mainObj[controlSum] != QJsonValue::Undefined && mainObj[controlSum].toInt(-1) >= 0)
    {
        obj = mainObj[viewStr].toObject();
        if(obj.count() == (mainObj[controlSum].toInt() / obj.size() / 3))
            return convertJsonObjectToPriceUpgradesMap(obj, view);
        else return false;
    }
    else return false;
}

bool Keeper::loadPrototypes(const QJsonObject & mainObj, QJsonArray & arr, Army * army, const QString & prototype) const
{
    if(mainObj[prototype] != QJsonValue::Undefined && mainObj[prototype].isArray())
    {
        arr = mainObj[prototype].toArray();
        if(arr.size() == TROOP_TYPES_COUNT)
            return convertJsonArrayToPrototypes(arr, army);
        else return false;
    }
    else return false;
}

bool Keeper::loadTown(const QJsonObject & mainObj, QJsonObject & obj, Town * town, const QString & townStr) const
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

void Keeper::convertArmyToJsonArray(const QVector<Troop *> & army, QJsonArray & arr) const
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

bool Keeper::convertJsonArrayToArmy(const QJsonArray & arr, Army * army) const
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

bool Keeper::convertJsonObjectToPriceUpgradesMap(const QJsonObject & obj, View * view) const
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

bool Keeper::convertJsonArrayToPrototypes(const QJsonArray & arr, Army * army) const
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

bool Keeper::convertJsonObjectToTown(const QJsonObject & obj, Town * town) const
{
    if(obj["health"] != QJsonValue::Undefined && obj["health"].toInt(-1) >= 0 && obj["health"].toInt(-1) <= TOWN_HEALTH)
        town->health = obj["health"].toInt();
    else return false;

    if(obj["income"] != QJsonValue::Undefined && obj["income"].toInt(-1) >= TOWN_INCOME)
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

bool Keeper::convertJsonObjectToTroop(const QJsonObject & o, Troop * t) const
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

bool Keeper::isLastGameExists() const
{
    QFile file;
    file.setFileName(LAST_GAME_FILE_PATH);
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
    file.close();
    return true;
}

void Keeper::convertTownToJsonObject(const Town * town, QJsonObject & obj) const
{
    obj.insert("health", town->health);
    obj.insert("income", town->income);
    obj.insert("money", town->money);
    obj.insert("timer_remainingTime", town->incomeTimer_remainingTime);
}

void Keeper::earnedMoneyP1Plus(const int & income)
{
    earnedMoneyP1 += income;
}

void Keeper::earnedMoneyP2Plus(const int & income)
{
    earnedMoneyP2 += income;
}

void Keeper::wastedMoneyP1Plus(const int & wasted)
{
    wastedMoneyP1 += wasted;
}

void Keeper::wastedMoneyP2Plus(const int & wasted)
{
    wastedMoneyP2 += wasted;
}

void Keeper::countOfUnitsP1Plus()
{
    countOfUnitsP1++;
}

void Keeper::countOfUnitsP2Plus()
{
    countOfUnitsP2++;
}

void Keeper::countOfModificationP1Plus()
{
    countOfModificationP1++;
}

void Keeper::countOfModificationP2Plus()
{
    countOfModificationP2++;
}

void Keeper::winP1()
{
    winner = gamerNameP1;
}

void Keeper::winP2()
{
    winner = gamerNameP2;
}

void Keeper::removeSettingsFile() const
{
    QFile(SETTING_FILE_PATH).remove();
}
