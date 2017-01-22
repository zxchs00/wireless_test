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
}

Output::~Output()
{
    delete ui;
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
    }

    // status set
    QList<QLineEdit*> input = this->findChildren<QLineEdit*>();
    for(int i=0;i<input.length();i++){
        input.at(i)->setDisabled(!(this->starting));
    }
    this->starting = !(this->starting);
}
