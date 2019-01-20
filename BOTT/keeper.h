#ifndef KEEPER_H
#define KEEPER_H

#include <QObject>
#include <QTime>
#include <QtSql/QSqlDatabase>

class Troop;

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

    bool saveLastGame(QVector<Troop *> army1, QVector<Troop *> army2, QVector<Troop *> prototypes1, QVector<Troop *> prototypes2, QVector<int> town1, QVector<int> town2 );
    bool saveSettings(const QMap<QString, Qt::Key> & settings1, const QMap<QString, Qt::Key> & settings2, const int musicVolume);
    bool saveStatistics();
    int getCountOfGamesRecords(bool refreshCount = false);
    int loadMusicVolume();
    QVector<int> * loadSettings(QMap<QString, Qt::Key> & settings, int id); // id - это номер view, для которого нужно загрузить настройки = 0 или 1
    QSqlQuery & getGamesRecords();
    QSqlQuery & getRecordAboutGamer(int id);
    QString & getWinner();
    void refreshStatistics();
    void removeSettingsFile();
    void setGamersNames(QString gamer1, QString gamer2);

private:
    bool connectDB();

public slots:
    // Слоты для сбора статистики
    void countOfModificationP1Plus();
    void countOfModificationP2Plus();
    void countOfUnitsP1Plus();
    void countOfUnitsP2Plus();
    void earnedMoneyP1Plus(int income);
    void earnedMoneyP2Plus(int income);
    void wastedMoneyP1Plus(int wasted);
    void wastedMoneyP2Plus(int wasted);
    void winP1();
    void winP2();

signals:
    void requiredShowMes(QString text);

};

#endif // KEEPER_H
