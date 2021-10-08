#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include <QWidget>
#include <QtMath>
#include <random>

typedef std::mt19937 RngEngine;

class Column
{
public:
    explicit Column(){}
    int mostvalue;
    int mostcount;
    int leastvalue;
    int leastcount;
    int occurence[60];
    int *mininterval;
    int *maxinterval;
    int *delta;
    int *currInterval;
    QVector<int> probablyValues;
    QVector<int> quadrant;
    QVector<int> nextquadrant;
};

const int firstQvalues[] = {1, 2, 3, 4, 5, 11, 12, 13, 14, 15, 21, 22, 23, 24, 25};
const int secondQvalues[] = {6, 7, 8, 9, 10, 16, 17 ,18 ,19, 20, 26, 27, 28, 29, 30};
const int thirdQvalues[] = {36, 37, 38, 39, 40, 46, 47, 48, 49, 50, 56, 57, 58, 59, 60};
const int fourthQvalues[] = {31, 32, 33, 34, 35, 41, 42, 43, 44, 45, 51, 52, 53, 54, 55};

class DataProcessing : public QWidget
{
    Q_OBJECT
public:
    explicit DataProcessing(int vectCount, QWidget *parent = 0);
    ~DataProcessing();

    // preparation of the rotation vectors
    void initFFT( void);
    // input window
    void windowFFT(float *s);
    // fast fourier transform
    void FFT( void);
    // compute power and scale output
    void powerScale(float *r);
    int checkPower2(int num);
    int getSize();
    void calcNumOccurrences(QVector<int> &list, int until);
    void calcIntervalsMean(QVector<int> &list, int until, bool isMean);
    int genPass(QVector<int> &list, int nextquad);
    void findQuadrants(QVector<int> &list);
    void nextQuadrantEstatistical(int qdrnt);
    void nextQuadrantEstatistical();
    void showIntervalsOf(int num, QVector<int> &value);

    const float PI2N;

    Column column;
    bool allprocess;
    int occurences;

private:


    // input complex vector
    float   *xr;
    float   *xi;

    // rotation vectors
    float   *wr;
    float   *wi;

    // bit reversal vector
    int   *rev;

    // window
    float   *ww;

    int inCount;

    RngEngine m_rngEngine;

signals:

public slots:
};

#endif // DATAPROCESSING_H
