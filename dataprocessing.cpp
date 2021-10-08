#include "dataprocessing.h"
#include <QDebug>

DataProcessing::DataProcessing(int vectCount, QWidget *parent) : QWidget(parent),
    PI2N(2 * M_PI/vectCount)
{
    inCount = checkPower2(vectCount);

    column.delta = new int[vectCount];
    column.currInterval = new int[vectCount];
    column.maxinterval = new int[vectCount];
    column.mininterval = new int[vectCount];

    if(!inCount)
        inCount = vectCount;

    xr = new float[inCount];
    xi = new float[inCount];
    wr = new float[inCount/2];
    wi = new float[inCount/2];
    ww = new float[inCount];
    rev = new int[inCount/2];

    initFFT();
    allprocess = false;
}

DataProcessing::~DataProcessing()
{
    delete column.currInterval;
    delete column.delta;
    delete xr;
    delete xi;
    delete wr;
    delete wi;
    delete ww;
    delete rev;
}

// apply Hann window to input vector
void DataProcessing::windowFFT(float *s)
{
    // apply window to input signal
    for(int i = 0; i < inCount; i++)
    {
        xr[i] = ((s[i] - 128) * ww[i]);
        xi[i] = 0.0;
    } // for i
} // windowFFT


int DataProcessing::checkPower2(int num)
{
    uint z = num;
    if(z & (z - 1))
    {
        for(z = 1; z < num; z = z<<1);
        return z;
    }
    else
    {
        return 0;
    }
}

int DataProcessing::getSize()
{
    return inCount;
}

void DataProcessing::calcNumOccurrences(QVector<int> &list, int until)
{
    int upto = until-1;
    int c, nmax, nmin, nmaxoc, nminoc;
    memset(column.occurence, 0, sizeof(int)*60);
    if(upto < 1139)
        return;
    for(int k = 1; k < 61; k++)
    {
        for(int i = 1139; i < upto; i++)
        {
            if(list[i] == k)
                column.occurence[k-1]++;
        }
    }
    for(c = 0, nmax = 0, nmin = upto; c < 60; c++)
    {
        if(column.occurence[c] < nmin)
        {
            nmin = column.occurence[c];
            nminoc = c+1;
        }
        if(column.occurence[c] > nmax)
        {
            nmax = column.occurence[c];
            nmaxoc = c+1;
        }
//        qDebug() << "max = " << nmax << "  min = " << nmin << " occurence[" << c+1 << "]=" << column.occurence[c] <<endl;
//        if(column.occurence[c] > 0)
//            occurences++;
    }
    column.mostvalue = nmaxoc;
    column.mostcount = nmax;
    column.leastvalue = nminoc;
    column.leastcount = nmin;
//    qDebug() << "max = " << nmax << "  min = " << nmin << endl;
}

//void DataProcessing::calcIntervalsMean(QVector<int> &list)
//{
//    int maccum = 0, mlastcount = 0, laccum = 0, llastcount = 0;
//    int mdiv = 0, ldiv = 0;
//    int i;
//    for(i = 0; i < list.count(); i++)
//    {
//        if(list[i] == column.mostvalue)
//        {
//            qDebug() << "accum = " << maccum;
//            maccum += i - mlastcount;
//            mlastcount = i;
//            mdiv++;
//        }
//        if(list[i] == column.leastvalue)
//        {
//            laccum += i - llastcount;
//            llastcount = i;
//            ldiv++;
//        }
//    }
//    column.delta[0] = maccum/mdiv;
//    column.delta[1] = laccum/ldiv;
//    qDebug() << "deltam=" << column.delta[0] << "  maccum=" << maccum << " i=" << mdiv;
//    qDebug() << "deltal=" << column.delta[1] << "  laccum=" << laccum << " i=" << ldiv << endl;
//}

void DataProcessing::calcIntervalsMean(QVector<int> &list, int until, bool isMean)
{
    int upto = until;
    int accum = 0, lastcount = 0;
    int div = 0;
    int i, maxinterval = 0, mininterval = upto - 1139, delta = 10;
    if(upto < 1139)
        return;
    column.probablyValues.clear();

    for(int k = 1; k < 61; k++)
    {
        for(lastcount = i = 1139; i < upto; i++)
        {
            if(list[i] == k)
            {
                accum += i - lastcount;
                if(maxinterval < (i - lastcount))
                    maxinterval = i - lastcount;
                if(mininterval > (i - lastcount))
                    mininterval = i - lastcount;
                lastcount = i;
                div++;
            }
        }
//        qDebug() << "teste" << __LINE__ << __FILE__ << div;
        column.maxinterval[k-1] = maxinterval;
        column.mininterval[k-1] = mininterval;
        column.currInterval[k-1] = i - lastcount;
        if(div == 0)
            column.delta[k-1] = accum;
        else
            column.delta[k-1] = accum/div;
        div = 0; accum = 0; maxinterval = 0; mininterval = upto - 1139;
        int subt = column.delta[k-1] - column.currInterval[k-1];
//        qDebug() << "teste" << __LINE__ << __FILE__;
        if(isMean)
            subt = abs(subt);
//        if(subt < delta) comentado para mostrar a estatistica de todos numeros de cada coluna
//        {
//            delta = abs(column.currInterval[k-1] - column.delta[k-1]);
            column.probablyValues.append(k);
//        }
//        qDebug() << "teste" << __LINE__ << __FILE__;
    }
    if(!allprocess)
    {
        qDebug() << "valor deve ser jogado  ";
        for (int k = 0; k < column.probablyValues.count(); k++)
            qDebug() << column.probablyValues[k] << " (" << column.occurence[column.probablyValues[k]-1] << ")"
                     << "  curr interval->" << column.currInterval[column.probablyValues[k]-1]
                     << "  max interval->" << column.maxinterval[column.probablyValues[k]-1]
                     << "  min interval->" << column.mininterval[column.probablyValues[k]-1]
                     << "  mean intervals->" << column.delta[column.probablyValues[k]-1]
                     << "  diff->" << column.delta[column.probablyValues[k]-1]-column.currInterval[column.probablyValues[k]-1];

        qDebug() << "  mais siu->" << column.mostvalue << "  menos saiu->" << column.leastvalue;
    }
}

void DataProcessing::showIntervalsOf(int num, QVector<int> &value)
{
    if(!allprocess)
    {
        qDebug() << " value->" << " (" << column.occurence[value[num-1]-1] << ")"
                << value[num-1] << " currinterval->" << column.currInterval[value[num-1]-1] << "  diff->" << column.delta[value[num-1]-1]-column.currInterval[value[num-1]-1]
                << " max inerval->" << column.maxinterval[value[num-1]-1] << " min interval->" << column.mininterval[value[num-1]-1];
    }
}

int DataProcessing::genPass(QVector<int> &list, int nextquad)
{
    uint passLen = 1;
    int ret, two = rand();
    const int *pvector = 0;
    bool pass = false;
    Q_ASSERT(passLen > 0);

    ////////////////////
    //Charset
    ////////////////////
    QString m_charset("0123456789");

    if(list.count() > 9)
        passLen = 2;

    if(nextquad == 1)
        pvector = firstQvalues;
    else if(nextquad == 2)
        pvector = secondQvalues;
    else if(nextquad == 3)
        pvector = thirdQvalues;
    else if(nextquad == 4)
        pvector = fourthQvalues;
    else
        return 0;

    start:
    //////////
    // Rand()
    //////////
    if(two&1)
    {
        QString password("");

        for(uint i = 0; i < passLen; ++i)
        {
            password[i] = m_charset[rand()%m_charset.size()];
        }
        ret = password.toInt();

    }
    ////////////
    // Mersenne
    ////////////
    else
    {
        std::uniform_int_distribution<> dist(0, m_charset.size()-1);
        QString rChars;
        rChars.resize(passLen);
        for(uint i = 0; i < passLen; ++i)
        {
            rChars[i] = m_charset[dist(m_rngEngine)];
        }
       ret = rChars.toInt();
    }
    if(ret < list.count())
    {
        ret = list.at(ret);
        for(int k = 0; k < 15; k++)
        {
            if(pvector[k] == ret)
            {
                pass = true;
                break;
            }
        }
        if(pass == false)
            goto start;
    }
    else
        goto start;

    return ret;
}

void DataProcessing::findQuadrants(QVector<int> &list)
{
    column.quadrant.clear();
    for(int k = 1139; k < list.count(); k++)
    {
        int l = list.at(k);
        for(int i = 1, score = 0; i < 26; i++)
        {
            if(l == i)
            {
                column.quadrant.append(1);
                break;
            }
            else if(l == (i+30))
            {
                column.quadrant.append(4);
                break;
            }
            if(i-score == 5){i += 5; score+=10;}
        }
        for(int i = 6, score = 0; i < 31; i++)
        {
            if(l == i)
            {
                column.quadrant.append(2);
                break;
            }
            else if(l == (i+30))
            {
                column.quadrant.append(3);
                break;
            }
            if(i-score == 10){i += 5; score+=10;}
        }
    }
}

void DataProcessing::nextQuadrantEstatistical(int qdrnt)
{
    for(int i = 0; i < column.quadrant.count()-1; i++)
    {
        if(column.quadrant.at(i)==qdrnt)
        {
            column.nextquadrant.append(column.quadrant.at(i+1));
        }
    }
}

void DataProcessing::nextQuadrantEstatistical()
{
    int fq = 0, sq = 0, tq = 0, qq = 0;
    for(int i = 0; i < column.quadrant.count(); i++)
    {
        int now = column.quadrant.at(i);
        if(now == 1)
            fq++;
        else if(now == 2)
            sq++;
        else if(now == 3)
            tq++;
        else if(now == 4)
            qq++;
    }
    qDebug() << "primeiro quad-> " << fq << " segundo quad-> " << sq;
    qDebug() << "terceiro quad-> " << tq << " quarto quad-> " << qq;
}

void convolve (double *p_coeffs, int p_coeffs_n,
               double *p_in, double *p_out, int n)
{
  int i, j, k;
  double tmp;

  for (k = 0; k < n; k++)  //  position in output
  {
    tmp = 0;

    for (i = 0; i < p_coeffs_n; i++)  //  position in coefficients array
    {
      j = k - i;  //  position in input

      if (j >= 0)  //  bounds check for input buffer
      {
        tmp += p_coeffs [k] * p_in [j];
      }
    }

    p_out [i] = tmp;
  }
}
