#include "mysniff.h"

MySniff::MySniff(QObject *parent) : QObject(parent)
{

}

MySniff::~MySniff()
{
    std::cout << "bye" << std::endl;
}

void MySniff::run(const std::string &iface)
{
    SnifferConfiguration config;
    config.set_promisc_mode(true);
    config.set_filter("type mgt subtype beacon");
    config.set_rfmon(true);
    Sniffer sniffer(iface, config);
    std::cout << "hi" << std::endl;
    sniffer.sniff_loop(make_sniffer_handler(this, &MySniff::callback));
}

bool MySniff::callback(PDU &pdu)
{
    // Get the Dot11 layer
    const Dot11Beacon& beacon = pdu.rfind_pdu<Dot11Beacon>();
    // All beacons must have from_ds == to_ds == 0
    if (!beacon.from_ds() && !beacon.to_ds()) {
        // Get the AP address
        address_type addr = beacon.addr2();
        int ch = beacon.ds_parameter_set();
        // Look it up in our set
        //ssids_type::iterator it = ssids.find(addr);
        try {
            std::string ssid = beacon.ssid();

            ssids.insert(addr);
            emit add_new(QString::fromStdString(addr.to_string()), ch, QString::fromStdString(ssid));

        }
        catch (std::runtime_error& e) {
            // No ssid, just ignore it.
            emit print_error(e);
            // error exception
        }
    }
    return true;
}
