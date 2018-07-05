#include "sierracalc.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SierraCalc w;
    w.show();

    return a.exec();
}
