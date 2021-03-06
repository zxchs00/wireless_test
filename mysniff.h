#ifndef MYSNIFF_H
#define MYSNIFF_H

#include <QObject>
#include <set>
#include <string>
#include <tins/tins.h>

using namespace Tins;

class MySniff : public QObject
{
    Q_OBJECT
public:
    explicit MySniff(QObject *parent = 0);
    ~MySniff();

signals:
    void add_new(QString addr, int ch, QString ssid);
    void print_error(std::runtime_error& e);

public slots:
    void run(const std::string& iface, QString ch, QString bs);
    bool callback(PDU& pdu);

private:
    typedef Dot11::address_type address_type;
    typedef std::set<address_type> ssids_type;
    ssids_type ssids;
    QString f_ch;
    QString f_bs;
};

#endif // MYSNIFF_H
