#include "output.h"
#include "ui_output.h"


Output::Output(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Output)
{
    ui->setupUi(this);
    setWindowTitle("Airodump-LHS");

    toSniff = new QTimer();
    QObject::connect(toSniff,SIGNAL(timeout()),this,SLOT(startSniff()));

    ui->ap->horizontalHeader()->setStretchLastSection(true);
    ui->ap->horizontalHeader()->resizeSection(0,180);
    ui->ap->setEditTriggers(QAbstractItemView::NoEditTriggers);

    qRegisterMetaType<std::string>();
}

Output::~Output()
{
    hello->quit();
    delete ui;
}

void Output::startSniff()
{
    this->toSniff->stop();
    ui->ap->setRowCount(0);
    noye = new MySniff();
    try{
        hello = new QThread();
        noye->moveToThread(hello);
        //workerThread.start();
        //connect(&workerThread, &QThread::finished, noye, &QObject::deleteLater);

        connect(this,SIGNAL(start_sniff(const std::string&,QString,QString)),noye,SLOT(run(const std::string&,QString,QString)));
        connect(noye,SIGNAL(add_new(QString,int,QString)),this,SLOT(new_add(QString,int,QString)));
        connect(noye,SIGNAL(print_error(std::runtime_error&)),this,SLOT(error_print(std::runtime_error&)));

        hello->start();
        emit start_sniff(ui->dev->text().toStdString(), ui->ch->text(), ui->bssid->text());
    }
    catch (std::runtime_error& e) {
        noye->~QObject();
        QMessageBox* a = new QMessageBox();
        a->setWindowTitle("Warning!");
        a->setText(QString(e.what()));
        a->exec();
        this->on_start_clicked();
        return;
    }

}

void Output::on_start_clicked()
{
    // if you want to stop
    if(this->starting){
        ui->start->setText("Start");
        noye->~MySniff();
        hello->terminate();
    }

    // if you want to start
    else{
        if(ui->dev->text() == ""){
            QMessageBox* a = new QMessageBox();
            a->setWindowTitle("Warning!");
            a->setText("Please Input Device name!");
            a->exec();
            return;
        }

        ui->start->setText("Stop");

        //QString a = ui->dev->text();
        //ui->ap2st->setText(a);

        // start Qtimer to start sniffing at other thread
        toSniff->start(200);
    }

    // status setting
    QList<QLineEdit*> input = this->findChildren<QLineEdit*>();
    for(int i=0;i<input.length();i++){
        input.at(i)->setDisabled(!(this->starting));
    }
    this->starting = !(this->starting);
}

void Output::new_add(QString addr, int ch, QString ssid)
{
    QList<QTableWidgetItem*> exist = ui->ap->findItems(addr,Qt::MatchExactly);
    if(exist.empty()){
        int row = ui->ap->rowCount();
        ui->ap->insertRow(row);
        ui->ap->setItem(row,0,new QTableWidgetItem(addr));
        QTableWidgetItem* tmp = new QTableWidgetItem();
        tmp->setData(Qt::EditRole, 1);
        ui->ap->setItem(row,1,tmp);
        ui->ap->setItem(row,2,new QTableWidgetItem(QString::number(ch)));
        ui->ap->setItem(row,3,new QTableWidgetItem(ssid));
    }
    else{
        QTableWidgetItem* beaconCount = ui->ap->item(exist.at(0)->row(),1);
        beaconCount->setData(Qt::EditRole, beaconCount->data(Qt::EditRole).toInt() + 1);
    }
}

void Output::error_print(std::runtime_error &e)
{
    QMessageBox* a = new QMessageBox();
    a->setWindowTitle("Warning!");
    a->setText(QString(e.what()));
    a->exec();
    this->on_start_clicked();
    return;
}
