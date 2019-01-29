#include "constants.h"
#include "databasehandler.h"
#include "keeper.h"
#include <QSqlQuery>
#include <QVariant>


DatabaseHandler::DatabaseHandler(Keeper * parent)
{
    this->parent = parent;
    db = QSqlDatabase::addDatabase("QSQLITE");
    connectDB();
    countOfGamesRecords = -1;
}

DatabaseHandler::~DatabaseHandler()
{
    db.close();
}

bool DatabaseHandler::saveStatistics() const
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO games (time, gamer1_id, gamer2_id) VALUES (:time, :gamer1_id, :gamer2_id)");
    query.bindValue(":time", parent->gameDuration.msecsTo(QTime::currentTime()));
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

    if(!parent->winner.isEmpty())
    {
        if(parent->winner == parent->gamerNameP1)
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

    player1.bindValue(":name", parent->gamerNameP1);
    player1.bindValue(":earnedMoney", parent->earnedMoneyP1);
    player1.bindValue(":wastedMoney", parent->wastedMoneyP1);
    player1.bindValue(":countOfUnits", parent->countOfUnitsP1);
    player1.bindValue(":countOdMods", parent->countOfModificationP1);
    player2.bindValue(":name", parent->gamerNameP2);
    player2.bindValue(":earnedMoney", parent->earnedMoneyP2);
    player2.bindValue(":wastedMoney", parent->wastedMoneyP2);
    player2.bindValue(":countOfUnits", parent->countOfUnitsP2);
    player2.bindValue(":countOdMods", parent->countOfModificationP2);

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

int DatabaseHandler::getCountOfGamesRecords(const bool & refreshCount)
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

QSqlQuery & DatabaseHandler::getGamesRecords() const
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

QSqlQuery & DatabaseHandler::getRecordAboutGamer(const int & id) const
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

bool DatabaseHandler::connectDB()
{
    db.setDatabaseName(keeper::DATABASE_FILE_PATH);

    if(!QFile::exists(keeper::DATA_DIR_PATH))
        QDir().mkdir(keeper::DATA_DIR_PATH);

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
