#ifndef KEEPER_H
#define KEEPER_H

#include <QObject>
#include <QTime>
#include <QtSql/QSqlDatabase>

class Army;
class Town;
class Troop;
class View;

class Keeper : public QObject
{
    Q_OBJECT

    int countOfGamesRecords;

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

    QSqlDatabase db;
    QString gamerNameP1;
    QString gamerNameP2;
    QString winner;
    QTime gameDuration;

public:
    explicit Keeper(QObject * parent = nullptr);
    ~Keeper();

    bool checkExistsOfFieldsAndControlSum(const QJsonObject & mainObj, const QString & field1, const QString & field2, const QString & sum) const;
    bool convertJsonArrayToArmy(const QJsonArray & arr, Army * army) const;
    bool convertJsonObjectToPriceUpgradesMap(const QJsonObject & obj, View * view) const;
    bool convertJsonArrayToPrototypes(const QJsonArray & arr, Army * army) const;
    bool convertJsonObjectToTown(const QJsonObject & obj, Town * town) const;
    bool convertJsonObjectToTroop(const QJsonObject & obj, Troop * troop) const;
    bool isLastGameExists() const;
    bool loadLastGame(Army * army1, Army * army2,
                      Town * town1, Town * town2,
                      View * view1, View * view2);
    bool saveLastGame(const QVector<Troop *> & army1, const QVector<Troop *> & army2,
                      const QVector<Troop *> & prototypes1, const QVector<Troop *> & prototypes2,
                      const Town * town1, const Town * town2,
                      const QMap<QString, int> & upgrades1, const QMap<QString, int> & upgrades2) const;
    bool saveSettings(const QMap<QString, Qt::Key> & settings1, const QMap<QString, Qt::Key> & settings2, const int & musicVolume) const;
    bool saveStatistics() const;
    int getCountOfGamesRecords(const bool & refreshCount = false);
    int loadMusicVolume() const;
    QVector<int> * loadSettings(QMap<QString, Qt::Key> & settings, const int & id) const; // id - это номер view, для которого нужно загрузить настройки = 0 или 1
    QSqlQuery & getGamesRecords() const;
    QSqlQuery & getRecordAboutGamer(const int & id) const;
    const QString & getWinner() const;
    void convertArmyToJsonArray(const QVector<Troop *> & army, QJsonArray & arr) const;
    void convertTownToJsonObject(const Town * town, QJsonObject & obj) const;
    void refreshStatistics();
    void removeLastGameFile() const;
    void removeSettingsFile() const;
    void setGamersNames(const QString & gamer1, const QString & gamer2);

private:
    bool connectDB();
    bool loadArmy(const QJsonObject & mainObj, QJsonArray & arr, Army * army, const QString & arm, const QString & controlSum) const;
    bool loadPriceUpgrades(const QJsonObject & mainObj, QJsonObject & obj, View * view, const QString & viewStr, const QString & controlSum) const;
    bool loadPrototypes(const QJsonObject & mainObj, QJsonArray & arr, Army * army, const QString & prototype) const;
    bool loadTown(const QJsonObject & mainObj, QJsonObject & obj, Town * town, const QString & townStr) const;

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
