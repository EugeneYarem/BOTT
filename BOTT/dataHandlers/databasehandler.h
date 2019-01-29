#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QtSql/QSqlDatabase>

class Keeper;

class DatabaseHandler : public QObject
{
    Q_OBJECT

    int countOfGamesRecords;
    Keeper * parent;
    QSqlDatabase db;

public:
    DatabaseHandler(Keeper * parent);
    ~DatabaseHandler();

    bool saveStatistics() const;
    int getCountOfGamesRecords(const bool & refreshCount = false);
    QSqlQuery & getGamesRecords() const;
    QSqlQuery & getRecordAboutGamer(const int & id) const;

private:
    bool connectDB();

signals:
    void requiredShowMes(const QString & text) const;

};

#endif // DATABASEHANDLER_H
