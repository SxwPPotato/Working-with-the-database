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

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_act_addData_triggered()
{
    dataDb->show();
}

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

void MainWindow::on_pb_request_clicked()
{
    QString typeRequest = ui->cb_category->currentText();

    if(typeRequest == "Все") {
        auto req = [&]{dataBase->RequestToDB(request_all);};

        QtConcurrent::run(req);


    }

    else if(typeRequest == "Комедия"){


        auto req = [&]{dataBase->RequestToDB(request_comedy);};

        QtConcurrent::run(req);
    }

    else if(typeRequest == "Ужасы"){


        auto req = [&]{dataBase->RequestToDB(request_horror);};

        QtConcurrent::run(req);
    }
}

void MainWindow::ScreenDataFromDB(const QTableWidget *widget)
{

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

}


void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->act_connect->setText("Отключиться");
        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");
        ui->pb_request->setEnabled(true);

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

void MainWindow::ReceiveStatusRequestToDB(QSqlError err)
{

    if(err.type() != QSqlError::NoError){
        msg->setText(err.text());
        msg->exec();
    }
    else{

        dataBase->ReadAnswerFromDB();

    }

}

void MainWindow::on_pb_clear_clicked()
{
    for(int i = ui->tb_result->rowCount() - 1; i >= 0; i--){
    ui->tb_result->removeRow(i);
    }
    for(int j = ui->tb_result->columnCount() - 1; j >= 0; j--){
    ui->tb_result->removeColumn(j);
    }
}

