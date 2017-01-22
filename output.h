#ifndef OUTPUT_H
#define OUTPUT_H

#include <QWidget>
#include <QTimer>
#include <QTableWidget>
#include <QMessageBox>

#include <tins/tins.h>

#include "iamsniffer.h"

using namespace Tins;

namespace Ui {
class Output;
}

class Output : public QWidget
{
    Q_OBJECT

public:
    explicit Output(QWidget *parent = 0);
    ~Output();

private slots:
    bool handler(PDU& pdu);
    void startSniff();
    void on_start_clicked();

private:
    typedef Dot11::address_type address_type;
    Ui::Output *ui;
    bool starting = false;
    QTimer* toSniff;
    //QString apTitle = "BSSID\tbeacon\tSSID\n";
};

#endif // OUTPUT_H
