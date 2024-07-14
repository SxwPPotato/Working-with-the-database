#ifndef DATABASE_H
#define DATABASE_H

#include <QTableWidget>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#define DB_NAME "QPSQL"
#define NUM_DATA_FOR_CONNECT_TO_DB 5


enum fieldsForConnect{

    hostName = 0,
    dbName = 1,
    login = 2,
    pass = 3,
    port = 4

};


enum answersType{

    requestAllFilms = 1,
    requestComedy = 2,
    requestHorrors = 3

};



class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void AddDataBase( QString nameDB = "");
    bool ConnectToDataBase(QVector<QString> dataForConnect);
    void DisconnectFromDataBase(QString nameDb = "");
    void RequestToDB(QString request);
    QSqlError GetLastError(void);

    void ReadAnswerFromDB();




signals:

    void sig_SendDataFromDB(const QTableWidget *tableWg);
    void sig_SendStatusConnection(bool);
    void sig_SendStatusRequest(QSqlError err);


private:

    QSqlDatabase* dataBase;

    QSqlQuery* simpleQuery;

    QTableWidget* tableWidget;

    QTableView* tableView;





};

#endif // DATABASE_H
