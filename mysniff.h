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

signals:
    void add_new(QString addr, QString ssid);
    void print_error(std::runtime_error& e);
    void add_exist(QString addr, QString ssid);

public slots:
    void run(const std::string& iface);
    bool callback(PDU& pdu);

private:
    typedef Dot11::address_type address_type;
    typedef std::set<address_type> ssids_type;
    ssids_type ssids;
};

#endif // MYSNIFF_H
