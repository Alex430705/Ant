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
#define Q		100			// количество
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
//    for (int count = 0; count < 200; ++count)
//    {
//        int i = 0; //откуда идем
//        double tmp = 0.0;
//        int n = 0;
//        QSet<int> tabu;
//        QVector<double> way;
//        tabu.insert(i);
//        while (tabu.size() < 5)
//        {
//            for (int j=0; j<5; ++j) //куда идем
//            {
//                if ( i != j && !tabu.contains(j))
//                {
//                    double sum = Sum(i, tau, ny, tabu); //знаменатель
//                    P[i][j] = 100.0 * ( pow(ny[i][j], alpha) * pow(tau[i][j], beta) ) / sum; //вероятность перехода
//                    tmp += P[i][j];
//                    vec.push_back(tmp);
//                }
//                else
//                {
//                    P[i][j] = 0.0;
//                    vec.push_back(P[i][j]);
//                }
//            }
//            QTime midnight(0,0,0);
//            qsrand(midnight.secsTo(QTime::currentTime()));
//            int myP = qrand() % 100 + 1;
//            QVector<double>::iterator iter = qUpperBound(vec.begin(), vec.end(), myP);

//            n = iter - vec.begin();
//            way.push_back(L[i][n]);
//            i = n;
//            tabu.insert(i);
//            tmp = 0.0;
//            vec.clear();
//        }
//        way.push_back(L[n][0]);
//        int lengthWay = 0;
//        vector<double> v = way.toStdVector();
//        lengthWay = accumulate(v.begin(), v.end(), lengthWay);
//        if (lengthWay < bestWay){
//            bestWay = lengthWay;
//            qDebug() << "Best way = " << bestWay;
//        }
//        //обновляем феромон
//        tabu.clear();
//        v.clear();
//        way.clear();
//    }

    QVector<int> tabu;
    vector<int> way;
    vector<double> v;
    double tmp = 0.0;
    int i=0;
    tabu.append(i);
    while(tabu.length() < 5)
    {
        for (int j=0; j<5; ++j)
        {
            if (i!=j && !tabu.contains(j))
            {
                double z = Sum(i, tau, ny, tabu); //знаменатель
                P[i][j] = 100.0 * ( pow(ny[i][j], ALPHA) * pow(tau[i][j], BETTA) ) / z; //вероятность перехода
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
    int sum = accumulate(way.begin(),way.end(), 0);
    if (sum < bestWay || bestWay < 0)
    {
        bestWay = sum;
        qDebug() << "best way = " << bestWay;
    }
    return a.exec();
}

double Sum(int index, double tau[][5], double ny[][5] , QVector<int> tabu)
{
    double sum = 0.0;
    for (int j = 0; j<5; ++j )
    {
        if (index != j && !tabu.contains(j))
            sum += (double)ny[index][j] * (double)tau[index][j];
    }
    return sum;
    aasdasdasd;
}
