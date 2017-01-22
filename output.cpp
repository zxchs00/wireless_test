#include "output.h"
#include "ui_output.h"

#include <tins/tins.h>

using namespace Tins;

Output::Output(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Output)
{
    ui->setupUi(this);
    setWindowTitle("Airodump-LHS");

    SnifferConfiguration config;
    config.set_snap_len(2000);
    config.set_promisc_mode(true);


    toSniff = new QTimer();
    QObject::connect(toSniff,SIGNAL(timeout()),this,SLOT(startSniff()));

}

Output::~Output()
{
    delete ui;
}

void Output::startSniff()
{
    this->toSniff->stop();
    ui->ap2st->setText("this is testing!");

}

void Output::on_start_clicked()
{
    // if you want to stop
    if(this->starting){
        ui->start->setText("Start");
        ui->ap->setText("end test");
    }

    // if you want to start
    else{
        ui->start->setText("Stop");
        ui->ap->setText("start test");

        QString a = ui->dev->text();
        ui->ap2st->setText(a);

        if(ui->dev->text() == ""){
            ui->ap->setText("Please Input Device");
            return;
        }

        // start Qtimer to start loop at other thread
        toSniff->start(200);
    }

    // status setting
    QList<QLineEdit*> input = this->findChildren<QLineEdit*>();
    for(int i=0;i<input.length();i++){
        input.at(i)->setDisabled(!(this->starting));
    }
    this->starting = !(this->starting);
}
