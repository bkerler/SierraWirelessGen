#ifndef SIERRACALC_H
#define SIERRACALC_H

#include <QtWidgets/QMainWindow>
#include "ui_sierracalc.h"

class SierraCalc : public QMainWindow
{
	Q_OBJECT

public:
	SierraCalc(QWidget *parent = 0);
	~SierraCalc();

private:
	Ui::SierraCalcClass ui;
	bool SierraCalc::GetChallenge(QString& challenge, int& mode, int& key);
};

#endif // SIERRACALC_H
