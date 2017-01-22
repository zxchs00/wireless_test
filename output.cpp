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
    ui->ap->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

Output::~Output()
{
    delete ui;
}

bool Output::handler(PDU& pdu) {
    // Get the Dot11 layer
    const Dot11Beacon& beacon = pdu.rfind_pdu<Dot11Beacon>();
    // All beacons must have from_ds == to_ds == 0
    if (!beacon.from_ds() && !beacon.to_ds()) {
        // Get the AP address
        address_type addr = beacon.addr2();
        std::string ssid = beacon.ssid();
        QList<QTableWidgetItem*> exist = ui->ap->findItems(QString::fromStdString(addr.to_string()),Qt::MatchContains);
        if(exist.empty()){
            int row = ui->ap->rowCount();
            ui->ap->insertRow(row);
            ui->ap->setItem(row,0,new QTableWidgetItem(QString::fromStdString(addr.to_string())));
            QTableWidgetItem* tmp = new QTableWidgetItem();
            tmp->setData(Qt::EditRole, 1);
            ui->ap->setItem(row,1,tmp);
            ui->ap->setItem(row,2,new QTableWidgetItem(QString::fromStdString(ssid)));
            std::cout << addr.to_string() << ssid << std::endl;
        }
        else{
            QTableWidgetItem* beaconCount = ui->ap->itemAt(exist.at(0)->row(),1);
            beaconCount->setData(Qt::EditRole, beaconCount->data(Qt::EditRole).toInt() + 1);
        }
    }
    return true;
}

void Output::startSniff()
{
    this->toSniff->stop();

    SnifferConfiguration config;
    config.set_snap_len(2000);
    config.set_promisc_mode(true);
    config.set_filter("wlan type mgt subtype beacon");
    config.set_rfmon(true);
    try{
        Sniffer sniffer(ui->dev->text().toStdString(), config);
        sniffer.sniff_loop(make_sniffer_handler(this,&Output::handler));
    }
    catch (std::runtime_error& e) {
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

        QString a = ui->dev->text();
        ui->ap2st->setText(a);

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
