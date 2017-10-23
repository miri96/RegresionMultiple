#ifndef REGMULT_H
#define REGMULT_H

#include <QMainWindow>

namespace Ui {
class RegMult;
}

class RegMult : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegMult(QWidget *parent = 0);
    ~RegMult();
    double puntos [6][3]; //This matrix will save the points provided by the user.
    double matriz[3][4]; // This matrix will save the points summations.
    double dispersion[6][2];// This matrix will save all dispersion results.
    double a0,a1,a2;// These three variables will save the found values for each plane equation term.
    double sr,st,sxy,r,yprom;// These variables will save error values and the "ypoints" average .
    int i,j,k;// Variables for control cycles.

public slots:
    void ejecutar();// Slot for run the method.
    void cargar();// Slot for save data from a table widget.
    void limpia();// Slot for clear all user interface fields.
private:
    Ui::RegMult *ui;
};

#endif // REGMULT_H
