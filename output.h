#ifndef OUTPUT_H
#define OUTPUT_H

#include <QWidget>
#include <QTimer>
#include <QTableWidget>
#include <QMessageBox>
#include <QObject>
#include <QThread>
#include <QString>

#include <tins/tins.h>

#include "mysniff.h"

Q_DECLARE_METATYPE(std::string)

using namespace Tins;

namespace Ui {
class Output;
}

class Output : public QWidget
{
    Q_OBJECT
    QThread workerThread;
public:
    explicit Output(QWidget *parent = 0);
    ~Output();
signals:
    void start_sniff(const std::string& iface);
public slots:

private slots:
    void startSniff();
    void on_start_clicked();
    void new_add(QString addr, QString ssid);
    void exist_add(QString addr, QString ssid);
    void error_print(std::runtime_error& e);

private:
    typedef Dot11::address_type address_type;
    Ui::Output *ui;
    bool starting = false;
    QTimer* toSniff;
    //QString apTitle = "BSSID\tbeacon\tSSID\n";
};

#endif // OUTPUT_H
