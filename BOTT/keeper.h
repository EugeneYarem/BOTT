#ifndef KEEPER_H
#define KEEPER_H

#include "dataHandlers/databasehandler.h"
#include "dataHandlers/jsonhandler.h"
#include <QObject>
#include <QTime>

class Army;
class Town;
class Troop;
class View;

class Keeper : public QObject
{
    Q_OBJECT

    DatabaseHandler * dbHandler;

    // Переменные, которые хранят данные для сохранения статистики
    // P1 - player 1, P2 - player 2
    int countOfModificationP1;
    int countOfModificationP2;
    int countOfUnitsP1;
    int countOfUnitsP2;
    int earnedMoneyP1;
    int earnedMoneyP2;
    int lastGameDuration;
    int wastedMoneyP1;
    int wastedMoneyP2;

    JsonHandler * jsonHandler;
    QString gamerNameP1;
    QString gamerNameP2;
    QString winner;
    QTime gameDuration;

public:
    explicit Keeper(QObject * parent = nullptr);
    ~Keeper();

    bool isLastGameExists() const;
    bool loadLastGame(Army * army1, Army * army2,
                      Town * town1, Town * town2,
                      View * view1, View * view2) const;
    bool saveLastGame(const QVector<Troop *> & army1, const QVector<Troop *> & army2,
                      const QVector<Troop *> & prototypes1, const QVector<Troop *> & prototypes2,
                      const Town * town1, const Town * town2,
                      const QMap<QString, int> & upgrades1, const QMap<QString, int> & upgrades2) const;
    bool saveSettings(const QMap<QString, Qt::Key> & settings1, const QMap<QString, Qt::Key> & settings2, const int & musicVolume) const;
    bool saveStatistics() const;
    const QString & getWinner() const;
    friend bool DatabaseHandler::saveStatistics() const;
    friend bool JsonHandler::loadLastGame(Army * army1, Army * army2, Town * town1, Town * town2, View * view1, View * view2) const;
    friend bool JsonHandler::saveLastGame(const QVector<Troop *> & army1, const QVector<Troop *> & army2,
                      const QVector<Troop *> & prototypes1, const QVector<Troop *> & prototypes2,
                      const Town * town1, const Town * town2,
                      const QMap<QString, int> & upgrades1, const QMap<QString, int> & upgrades2) const;
    int getCountOfGamesRecords(const bool & refreshCount = false) const;
    int loadMusicVolume() const;
    QSqlQuery & getGamesRecords() const;
    QSqlQuery & getRecordAboutGamer(const int & id) const;
    QVector<int> * loadSettings(QMap<QString, Qt::Key> & settings, const int & id) const; // id - это номер view, для которого нужно загрузить настройки = 0 или 1
    void refreshStatistics();
    void removeLastGameFile() const;
    void removeSettingsFile() const;
    void setGamersNames(const QString & gamer1, const QString & gamer2);

public slots:
    // Слоты для сбора статистики
    void countOfModificationP1Plus();
    void countOfModificationP2Plus();
    void countOfUnitsP1Plus();
    void countOfUnitsP2Plus();
    void earnedMoneyP1Plus(const int & income);
    void earnedMoneyP2Plus(const int & income);
    void wastedMoneyP1Plus(const int & wasted);
    void wastedMoneyP2Plus(const int & wasted);
    void winP1();
    void winP2();

signals:
    void requiredShowMes(const QString & text) const;

};

#endif // KEEPER_H
