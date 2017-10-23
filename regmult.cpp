#include "regmult.h"
#include "ui_regmult.h"
#include <QTableWidgetItem>// Class for allow the use of Items from Table Widget.
#include <QString>// Class for allow the use of text strings.

RegMult::RegMult(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegMult)
{
    ui->setupUi(this);
    connect(ui->inicio,SIGNAL(clicked()),this,SLOT(ejecutar()));
    connect(ui->limpiar,SIGNAL(clicked()),this,SLOT(ejecutar()));
    connect(ui->cp,SIGNAL(clicked()),this,SLOT(cargar()));
}
void RegMult::cargar(){// This slot will save the points from table widget to a matrix and calculate each summation, when the Load button been clicked.
    for(i=0;i<3;i++){// This cycle analyzes rows.
        for(j=0;j<4;j++){// This cycle analyzes columns.
            matriz[i][j]=0;// Clears each summation matrix position.
        }
    }
    for(i=0;i<6;i++){// This cycle will save the points provided by the user from the table widget into another matrix.
        QTableWidgetItem* posicionx1=ui->tabla->item(i,0);// Creates a temporal variable for save each item value from the first table column.
        QTableWidgetItem* posicionx2=ui->tabla->item(i,1);// Creates a temporal variable for save each item value from the secound table column.
        QTableWidgetItem* posiciony=ui->tabla->item(i,2);// Creates a temporal variable for save each item value from the third table column.
        if((!posicionx1)||(!posicionx2)||(!posiciony)){// If the table widget item isn't null (empty), the cycle continues.
            continue;
        }
        puntos[i][0]=posicionx1->text().toDouble();// Saves the value from table item to a matrix position (for "x1").
        puntos[i][1]=posicionx2->text().toDouble();// Saves the value from table item to a matrix position (for "x2").
        puntos[i][2]=posiciony->text().toDouble();// Saves the value from table item to a matrix position (for "y").
        matriz[0][0]++;// This matrix position counts and saves the points number.
        matriz[0][1]=matriz[0][1]+puntos[i][0];// This matrix position saves the "x1" summation.
        matriz[0][2]=matriz[0][2]+puntos[i][1];// This matrix position saves the "x2" summation.
        matriz[0][3]=matriz[0][3]+puntos[i][2];// This matrix position saves the "y" summation.
        matriz[1][1]=matriz[1][1]+(puntos[i][0]*puntos[i][0]);// This matrix position saves the squared "x1" summation.
        matriz[1][2]=matriz[1][2]+(puntos[i][0]*puntos[i][1]);// This matrix position saves the "x1*x2" summation.
        matriz[1][3]=matriz[1][3]+(puntos[i][0]*puntos[i][2]);// This matrix position saves the "x1*y" summation.
        matriz[2][2]=matriz[2][2]+(puntos[i][1]*puntos[i][1]);// This matrix position saves the squared "x2" summation.
        matriz[2][3]=matriz[2][3]+(puntos[i][1]*puntos[i][2]);// This matrix position saves the "x2*y" summation.
    }
    if(matriz[0][0]<4){// If the user introduced less than 4 points:
           ui->tabla->setDisabled(false);// The first table widget keeps activate, till the user adds at least 4 points.
    }
     else{// If the user added at least 4 points:
        ui->inicio->setDisabled(false);//Enables the start button.
        ui->cp->setDisabled(true);//Disables the Load button.
        ui->tabla->setDisabled(true);// Disables the first table widget.
    }
   ui->limpiar->setDisabled(false);// Enables the Clear button.
  }
void RegMult::ejecutar(){// This slot will run the method, when the Start button been clicked after data introduction.
    a0=0, a1=0, a2=0, sr=0, st=0, sxy=0, r=0;// These variables starts from zero.
       // Fill the opposite matriz positions with the final values.
       matriz[1][0]=matriz[0][1];
       matriz[2][0]=matriz[0][2];
       matriz[2][1]=matriz[1][2];
       yprom=matriz[0][3]/matriz[0][0];// Calculates the "y" points average and saves it on "yprom".
       //Starts Simple Gaussian Elimination for find each plane equation term.
       double producto[4];// Creates a temporal array for save each product.
       for(k=0;k<2;k++){// This cycle turns to zero the matrix positions below the main diagonal.
           for(i=k+1;i<3;i++){// This cycle analyzes rows.
               producto[i]=matriz[i][k]/matriz[k][k];// Calculates each product for turn to zero the mentioned positions.
               for(j=0;j<5;j++){// This cycle analyzes columns.
                   matriz[i][j]=matriz[i][j]-(matriz[k][j]*producto[i]);// Calcuates the new matrix value for each position.
               }
           }
       }
       a2=matriz[2][3]/matriz[2][2];// Calculates the dependet term for x2.
       a1=(matriz[1][3]-(matriz[1][2]*a2))/(matriz[1][1]);// Calclates the dependent term for x1.
       a0=(matriz[0][3]-(matriz[0][2]*a2)-(matriz[0][1]*a1))/matriz[0][0];// Calculates the independent term for plane equation.
           //Finishes Simple Gaussian Elimination. The method continues.
       for(i=0;i<matriz[0][0];i++){// // This cycle calculates and saves dispersion results in a new matrix.
              dispersion[i][0]=(puntos[i][2]-yprom)*(puntos[i][2]-yprom);// Calculates the dispersion between points for each position.
              st=st+dispersion[i][0];// Calculates the dispersion between points summation.
              // Calculates the dispersion between each point and the found plane.
              dispersion[i][1]=(puntos[i][2]-a0-(a1*puntos[i][0])-(a2*puntos[i][1]))*(puntos[i][2]-a0-(a1*puntos[i][0])-(a2*puntos[i][1]));
              sr=sr+dispersion[i][1];// Calculates the dispersion between point and plane summation.
              ui->tabla2->setItem(i,0,new QTableWidgetItem(QString::number(dispersion[i][0])));// Show each dispersion between points value in the respective table widget position.
              ui->tabla2->setItem(i,1,new QTableWidgetItem(QString::number(dispersion[i][1])));// Show each dispersion point-plane in the respective table widget position.
       }
       sxy=sqrt((sr)/(matriz[0][0]-3));// Calculates the estimated standard error, and saves it on "sxy".
       r=sqrt((st-sr)/sr);// Calculates the linear correlation coefficient, and saves it on "r".
       QString temp1, temp2, temp3, temp4, temp5;// Creates temporal text variables.
       //Creates a text string for show the found plane equation and linear correlation coefficient value.
       temp1.append("La ecuacion del plano es: \n y= ").append(temp2.setNum(a0)).append("+").append
                   (temp3.setNum(a1)).append("(x1)+").append(temp4.setNum(a2)).append("(x2) \n").append
                   ("El coeficiente de correlacion (r)=").append(temp5.setNum(r));
           ui->rest->setText(temp1);// Show created string at text browser.
}
void RegMult::limpia(){// This slot will clear all user interface fields, when the Clear button been clicked.
    // Clears both table widgets.
    ui->tabla->clearContents();
    ui->tabla2->clearContents();
    ui->tabla->setDisabled(false);// Enables the first table widget.
    ui->rest->clear();// Clears the text browser.
    ui->cp->setDisabled(false);// Enables the Load button.
    ui->inicio->setDisabled(true);// Disables the start button.
    ui->limpiar->setDisabled(true);// Disables the clear button.
}
RegMult::~RegMult()
{
    delete ui;
}
