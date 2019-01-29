#include "constants.h"
#include "keeper.h"


Keeper::Keeper(QObject *parent) : QObject(parent)
{
    refreshStatistics();
    dbHandler = new DatabaseHandler(this);
    jsonHandler = new JsonHandler(this);
    connect(dbHandler, &DatabaseHandler::requiredShowMes, this, &Keeper::requiredShowMes);
}

Keeper::~Keeper()
{
    delete dbHandler;
    delete jsonHandler;
}

bool Keeper::loadLastGame(Army * army1, Army * army2, Town * town1, Town * town2, View * view1, View * view2) const
{
    return jsonHandler->loadLastGame(army1, army2, town1, town2, view1, view2);
}

bool Keeper::saveLastGame(const QVector<Troop *> & army1, const QVector<Troop *> & army2,
                          const QVector<Troop *> & prototypes1, const QVector<Troop *> & prototypes2,
                          const Town * town1, const Town * town2,
                          const QMap<QString, int> & upgrades1, const QMap<QString, int> & upgrades2) const
{
    return jsonHandler->saveLastGame(army1, army2, prototypes1, prototypes2, town1, town2, upgrades1, upgrades2);
}

QVector<int> * Keeper::loadSettings(QMap<QString, Qt::Key> & settings, const int & id) const
{
    return jsonHandler->loadSettings(settings, id);
}

bool Keeper::saveSettings(const QMap<QString, Qt::Key> & settings1, const QMap<QString, Qt::Key> & settings2, const int & musicVolume) const
{
    return jsonHandler->saveSettings(settings1, settings2, musicVolume);
}

bool Keeper::saveStatistics() const
{
    return dbHandler->saveStatistics();
}

int Keeper::getCountOfGamesRecords(const bool & refreshCount) const
{
    return dbHandler->getCountOfGamesRecords(refreshCount);
}

int Keeper::loadMusicVolume() const
{
    return jsonHandler->loadMusicVolume();
}

QSqlQuery & Keeper::getGamesRecords() const
{
    return dbHandler->getGamesRecords();
}

QSqlQuery & Keeper::getRecordAboutGamer(const int & id) const
{
    return dbHandler->getRecordAboutGamer(id);
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
    QFile(keeper::LAST_GAME_FILE_PATH).remove();
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

bool Keeper::isLastGameExists() const
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
    file.close();
    return true;
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
    QFile(keeper::SETTING_FILE_PATH).remove();
}
