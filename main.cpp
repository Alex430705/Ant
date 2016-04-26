#include <QCoreApplication>
#include <iostream>
#include <math.h>
#include <QDebug>
#include <QSet>
#include <QVector>
#include <QtAlgorithms>
#include <algorithm>
#include <QTime>

using namespace std;

#define ALPHA	1			// вес фермента
#define BETTA	1			// коэффициент эвристики
#define T_MAX	100			// время жизни колонии
#define M		20			// количество муравьев в колонии
#define Q		100.0			// количество
#define RHO		0.5			// коэффициент испарения феромона

double Sum(int index, double tau[][5], double ny[][5], QVector<int> tabu); //Суммирует ню и тау в знаменателе вероятностей

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QVector<double> vec;
    int bestWay = -1;

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

    double P[5][5] = {{0}};

    double ny[5][5] = {{0}};

    cout << "Matrix of distance:" << endl;
    for (int i=0; i < 5; ++i)
    {
        for (int j=0; j < 5; j++)
        {
            cout << L[i][j] << " ";
        }
        cout << endl ;
    }

    cout << endl << "Visibility:" << endl;

    for (int i =0; i<5; ++i)
    {
        for(int j = 0; j < 5; ++j)
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

    QVector<int> tabu;
    vector<int> way;
    vector<double> v;

    for (int k=0; k <20; ++k) //цикл по муравьям
    {
        int i=0;
        tabu.append(i);
        while(tabu.length() < 5) //посещенные вершины
        {
            for (int j=0; j<5; ++j) //куда идем
            {
                if (i!=j && !tabu.contains(j))
                {
                    double z = Sum(i, tau, ny, tabu); //знаменатель
                    P[i][j] = 100.0 * ( pow(tau[i][j], ALPHA) * pow(ny[i][j], BETTA) ) / z; //вероятность перехода
                    v.push_back(P[i][j]);
                }
                else
                {
                    P[i][j] = 0.0;
                    v.push_back(0.0);
                }
            }
            int myP = qrand() % 100 + 1;
            partial_sum(v.begin(), v.end(), v.begin());
            vector<double>::iterator iter = upper_bound(v.begin(),v.end(), myP);
            int to = iter - v.begin();
            way.push_back(L[i][to]);
            tabu.append(to);
            i = to;
            v.clear();
        }
        tabu.append(tabu.at(0));
        way.push_back(L[*(tabu.end()-2)][*(tabu.end()-1)]);
        int length = accumulate(way.begin(),way.end(), 0);
        if (length < bestWay || bestWay < 0)
        {
            bestWay = length;
            qDebug() << "best way = " << bestWay;
        }
        //остовляем феромон на пути муравья
        for( int i = 0; i < tabu.count(); ++i)
        {
            tau[i][i+1] += Q/length;
            tau[i+1][i] = tau[i][i+1];
        }
        for(int i=0; i<5; ++i)
        {
            for(int j=0; j<5; ++j)
            {
                cout << tau[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
        //испаряем феромон
        for (int i=0; i<5; ++i)
            for (int j=0; j<5; ++j)
               if (i != j) tau[i][j] *= (1-RHO);
        way.clear();
        tabu.clear();
    }
    return a.exec();
}

double Sum(int index, double tau[][5], double ny[][5] , QVector<int> tabu)
{
    double sum = 0.0;
    for (int j = 0; j<5; ++j )
    {
        if (index != j && !tabu.contains(j))
            sum += pow((double)tau[index][j],ALPHA) * pow((double)ny[index][j], BETTA);
    }
    return sum;
}
