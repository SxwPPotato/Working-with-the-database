#include "mainwindow.h"
#include "./ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lb_statusConnect->setStyleSheet("color:red");
    ui->pb_request->setEnabled(false);

    dataDb = new DbData(this);
    dataBase = new DataBase(this);
    msg = new QMessageBox(this);



    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);

    dataBase->AddDataBase(DB_NAME);

    connect(dataDb, &DbData::sig_sendData, this, [&](QVector<QString> receivData){
        dataForConnect = receivData;
    });


    connect(dataBase, &DataBase::sig_SendDataFromDB, this, &MainWindow::ScreenDataFromDB);

    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);
    connect(dataBase, &DataBase::sig_SendStatusRequest, this, &MainWindow::ReceiveStatusRequestToDB);


        this->setupModel(DB_NAME,
                             QStringList() <<("id")
                                           <<("Дата")
                                           <<("Время")
                       );


this->createUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::setupModel(const QString &tableName, const QStringList &headers)
{
    model = new QSqlTableModel(this);
    model->setTable(tableName);


    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
  }

void MainWindow::createUI()
{
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    model->select();
}



void MainWindow::on_act_addData_triggered()
{
    dataDb->show();
}

/*!
 * @brief Слот выполняет подключение к БД. И отображает ошибки.
 */

void MainWindow::on_act_connect_triggered()
{
    if(ui->lb_statusConnect->text() == "Отключено"){

        bool connectStatus = false;

        connectStatus =  dataBase->ConnectToDataBase(dataForConnect);

        if(connectStatus){

            ui->act_connect->setText("Отключиться");
            ui->lb_statusConnect->setText("Подключено к БД");
            ui->lb_statusConnect->setStyleSheet("color:green");
            ui->pb_request->setEnabled(true);

        }
        else{

            dataBase->DisconnectFromDataBase();
            msg->setIcon(QMessageBox::Critical);
            msg->setText(dataBase->GetLastError().text());
            msg->exec();

        }

    }
    else{

        dataBase->DisconnectFromDataBase();
        ui->lb_statusConnect->setText("Отключено");
        ui->act_connect->setText("Подключиться");
        ui->lb_statusConnect->setStyleSheet("color:red");
        ui->pb_request->setEnabled(false);
    }
}

/*!
 * \brief Обработчик кнопки "Получить"
 */
void MainWindow::on_pb_request_clicked()
{


    ///Тут должен быть код ДЗ

    auto req = [&]{dataBase->RequestToDB(request_comedy);};

    QtConcurrent::run(req);


}

/*!
 * \brief Слот отображает значение в QTableWidget
 * \param widget
 * \param typeRequest
 */
void MainWindow::ScreenDataFromDB(const QTableWidget *widget, int typeRequest)
{

    ///Тут должен быть код ДЗ

    switch (typeRequest) {

        case requestAllFilms:
        case requestHorrors:
        case requestComedy:{

            ui->tb_result->setRowCount(widget->rowCount( ));
            ui->tb_result->setColumnCount(widget->columnCount( ));
            QStringList hdrs;

            for(int i = 0; i < widget->columnCount(); ++i){
                hdrs << widget->horizontalHeaderItem(i)->text();
            }
            ui->tb_result->setHorizontalHeaderLabels(hdrs);

            for(int i = 0; i<widget->rowCount(); ++i){
                for(int j = 0; j<widget->columnCount(); ++j){
                    ui->tb_result->setItem(i,j, widget->item(i,j)->clone());
                }
            }

            ui->tb_result->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

            break;

        }
        default:
            break;
        }
}


void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->act_connect->setText("Отключиться");
        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");
        ui->pb_request->setEnabled(true);

         setupModel("QMYSQL",  QStringList() <<("id")<<("Дата") <<("Время") );
    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");
        msg->exec();
    }

}

/*!
 * \brief Метод обрабатывает ответ БД на поступивший запрос
 * \param err
 */
void MainWindow::ReceiveStatusRequestToDB(QSqlError err)
{

    if(err.type() != QSqlError::NoError){
        msg->setText(err.text());
        msg->exec();
    }
    else{

        dataBase->ReadAnswerFromDB(requestAllFilms);

    }



}

void MainWindow::on_pb_clear_clicked()
{
    ui->tb_result->clear();
}

