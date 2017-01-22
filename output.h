#ifndef OUTPUT_H
#define OUTPUT_H

#include <QWidget>

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

    void on_start_clicked();

private:
    Ui::Output *ui;
    bool starting = false;
};

#endif // OUTPUT_H
