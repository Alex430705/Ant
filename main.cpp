#include <QCoreApplication>
#include <iostream>
#include <math.h>
#include <QSet>
#include <QDebug>
#include <QVector>
#include <QtAlgorithms>

using namespace std;

double Sum(int index, double mas[][5], double tau[][5], double ny[][5]); //Суммирует ню и тау в знаменателе вероятностей

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSet<int> set;
    QVector<double> vec;

    double L[5][5] = { {0,38,74,59,45},
                       {38,0,46,61,72},
                       {74,46,0,49,85},
                       {59,61,49,0,42},
                       {45,72,85,42,0} };

    double tau[5][5] = { {0, 3, 2, 2, 2},
                         {3, 0, 1, 1, 1},
                         {2, 1, 0, 2, 2},
                         {2, 1, 2, 0, 1},
                         {2, 1, 2, 1, 0} };

    int tabu[5] = {0};

    double P[5][5] = {{0}};

    double alpha = 1.0 , beta = 1.0;
    double ny[5][5] = {{0}};

    cout << "Matrix of distance:" << endl;
    for (int i=0; i < 5; i++)
    {
        for (int j=0; j < 5; j++)
        {
            cout << L[i][j] << " ";
        }
        cout << endl ;
    }

    cout << endl << "Visibility:" << endl;

    for (int i =0; i<5; i++)
    {
        for(int j = 0; j<5; j++)
        {
            if (i != j)
                ny[i][j] = 1.0/L[i][j]; //Видимость
            else
                tau[i][j] = 0;
            cout << ny[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl << "Probabilities" << endl;

    int i = 0;
    double tmp = 0.0;

    for (int j=0; j<5; j++)
    {
        if ( i != j )
        {
            double sum = Sum(i, L, tau, ny);
            P[i][j] = 100.0 * ( pow(ny[i][j], alpha) * pow(tau[i][j], beta) ) / sum;
            cout << "P" << i+1 << j+1 << ": " << P[i][j] << endl;
            set << i;
            tmp += P[i][j];
            vec.push_back(tmp);
        }
    }

    double myP = qrand();
    vec.push_back(myP);
    qSort(vec);
    qFind(vec, myP);
    return a.exec();
}

double Sum( int index, double mas[][5], double tau[][5], double ny[][5] )
{
    double sum = 0.0;
    for (int j = 0; j<5; j++ )
    {
        if (index != j)
            sum += (double)ny[index][j] * (double)tau[index][j];
    }
    return sum;
}
