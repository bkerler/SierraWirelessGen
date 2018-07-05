#ifndef SIERRACALC_H
#define SIERRACALC_H

#include <QMainWindow>
#include "ui_sierracalc.h"

class SierraCalc : public QMainWindow, private Ui::SierraCalc
{
    Q_OBJECT

public:
    explicit SierraCalc(QWidget *parent = 0);
    ~SierraCalc();
private slots:
    void on_calcbutton_clicked();
};

#endif // SIERRACALC_H
