#include "regmult.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RegMult w;
    w.show();

    return a.exec();
}
