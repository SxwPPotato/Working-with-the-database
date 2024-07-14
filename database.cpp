#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    dataBase = new QSqlDatabase();

    simpleQuery = new QSqlQuery();

    tableWidget = new QTableWidget();

}

DataBase::~DataBase()
{
    delete dataBase;
}

void DataBase::AddDataBase( QString nameDB)
{
    *dataBase = QSqlDatabase::addDatabase(nameDB);
}

bool DataBase::ConnectToDataBase(QVector<QString> data)
{

    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());

    bool status;
    return dataBase->open(); 

    emit sig_SendStatusConnection(status);

}

void DataBase::DisconnectFromDataBase(QString nameDb)
{
    //Сначала выбираем базу данных
    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();

}

void DataBase::RequestToDB(QString request)
{


    *simpleQuery = QSqlQuery(*dataBase);

    QSqlError err;
    if(simpleQuery->exec(request) == false){
        err = simpleQuery->lastError();
    }


    emit sig_SendStatusRequest(err);


}




void DataBase::ReadAnswerFromDB()
{
        tableWidget->setColumnCount(3);
        tableWidget->setRowCount(0);
        QStringList hdrs;
        hdrs << "Название" << "Год выпуска" << "Жанр";
        tableWidget->setHorizontalHeaderLabels(hdrs);

        uint32_t conterRows = 0;

        while(simpleQuery->next()){
            QString str;
            tableWidget->insertRow(conterRows);

            for(int i = 0; i<tableWidget->columnCount(); ++i){

                str = simpleQuery->value(i).toString();

                    QTableWidgetItem *item = new QTableWidgetItem(str);
                    tableWidget->setItem(tableWidget->rowCount() - 1, i, item);

            }
            ++conterRows;
        }

        emit sig_SendDataFromDB(tableWidget);

}


QSqlError DataBase::GetLastError()
{
   return dataBase->lastError();
}



