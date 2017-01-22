#include "output.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Output w;
    w.show();

    return a.exec();
}
