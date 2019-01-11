#include "keeper.h"
#include "widget.h"
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QProcessEnvironment>
#include <QSqlQuery>
#include <QVariant>
#include <QVector>


Keeper::Keeper(QObject *parent) : QObject(parent)
{
    QProcessEnvironment env(QProcessEnvironment::systemEnvironment());
    dataDirPath = QDir::toNativeSeparators(env.value("USERPROFILE") + "/AppData/Local/Battle Of The Towns");
    dbFilePath = QDir::toNativeSeparators(env.value("USERPROFILE") + "/AppData/Local/Battle Of The Towns/statistics.sqlite");
    settingFilePath = QDir::toNativeSeparators(env.value("USERPROFILE") + "/AppData/Local/Battle Of The Towns/settings.json");

    db = QSqlDatabase::addDatabase("QSQLITE");
    connectDB();

    refreshStatistics();
    countOfGamesRecords = -1;
}

Keeper::~Keeper()
{
    db.close();
}

QVector<int> * Keeper::loadSettings(QMap<QString, Qt::Key> &settings, int id)
{
    QFile file;
    file.setFileName(settingFilePath);
    try
    {
        if(!file.open(QIODevice::ReadOnly))
            throw 1;
    }
    catch(int )
    {
        std::vector<int> errors = {0, 1, 2, 3, 4, 5, 6, 7, 8};
        return new QVector<int>(QVector<int>::fromStdVector(errors)); // ошибка: нет файла
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if(jsonDoc.isEmpty() || jsonDoc.isNull())
    {
        std::vector<int> errors = {0, 1, 2, 3, 4, 5, 6, 7, 8};
        return new QVector<int>(QVector<int>::fromStdVector(errors)); // ошибка: нет файла
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

bool Keeper::saveSettings(const QMap<QString, Qt::Key> & settings1, const QMap<QString, Qt::Key> & settings2, const int musicVolume)
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

    if(!QFile::exists(dataDirPath))
        QDir().mkdir(dataDirPath);

    QFile file;
    file.setFileName(settingFilePath);
    try
    {
        if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            throw -1;
    }
    catch(int )
    {
        return false;
    }

    file.write(jsonDoc.toJson());
    file.close();
    return true;
}

bool Keeper::saveStatistics()
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

QSqlQuery & Keeper::getGamesRecords()
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

QSqlQuery & Keeper::getRecordAboutGamer(int id)
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

int Keeper::getCountOfGamesRecords()
{
    if(countOfGamesRecords == -1)
    {
        QSqlQuery query = getGamesRecords();

        int res = 0;
        while (query.next())
            res++;

        countOfGamesRecords = res;
    }
    return countOfGamesRecords;
}

int Keeper::loadMusicVolume()
{
    QFile file;
    file.setFileName(settingFilePath);
    try
    {
        if(!file.open(QIODevice::ReadOnly))
            throw 1;
    }
    catch(int )
    {
        return -1; // ошибка: файла настроек нет
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if(jsonDoc.isEmpty() || jsonDoc.isNull())
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
    wastedMoneyP1 = 0;
    wastedMoneyP2 = 0;
    winner = "";
}

void Keeper::setGamersNames(QString gamer1, QString gamer2)
{
    gamerNameP1 = gamer1;
    gamerNameP2 = gamer2;
}

QString & Keeper::getWinner()
{
    return winner;
}

bool Keeper::connectDB()
{
    db.setDatabaseName(dbFilePath);

    if(!QFile::exists(dataDirPath))
        QDir().mkdir(dataDirPath);

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

void Keeper::earnedMoneyP1Plus(int income)
{
    earnedMoneyP1 += income;
}

void Keeper::earnedMoneyP2Plus(int income)
{
    earnedMoneyP2 += income;
}

void Keeper::wastedMoneyP1Plus(int wasted)
{
    wastedMoneyP1 += wasted;
}

void Keeper::wastedMoneyP2Plus(int wasted)
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
