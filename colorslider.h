#ifndef COLORSLIDER_H
#define COLORSLIDER_H

#include <QWidget>
#include <QDebug>
#include <algorithm>
#include <clickableqlabel.h>
#include "cdrawer.h"
#include "dataprocessing.h"
#include "pastraffle.h"
#include "qcustomplot.h"

#define FIRSTCOL    0
#define SECONDCOL   1
#define THIRDCOL    2
#define FOURTHCOL   3
#define FIFTHCOL   4
#define SIXTHCOL    5
#define NUMCOLUMNS  6

namespace Ui {
class ColorSlider;
}

class ColorSlider : public QWidget
{
    Q_OBJECT

public:
    explicit ColorSlider(QWidget *parent = 0);
    ~ColorSlider();
    void setupPlot(QVector<int> &parx);
    void setupPlot();
    void colorizeSequence(QVector<int> vect, int start, int finish, int column);
    void createGridNumbers(QList<QLabel *> *numbs, QWidget *tab);
    int stepLine(int step);
    int stepAnyLine(int step);

private slots:
    void on_horizontalSlider_sliderMoved(int position);
    void horzScrollBarChanged(int value);
    void vertScrollBarChanged(int value);
    void xAxisChanged(QCPRange range);
    void yAxisChanged(QCPRange range);

    void on_btnSelect_clicked();

    void on_pushButton_clicked();

    void on_btnCalculate_clicked();

    void on_btn_automate_clicked();

    void on_btnClearPanel_clicked();

    void on_btnSimulate_clicked();

    void on_btnRegister_clicked();

    void on_btnMake_clicked();

    void on_btnFoo_clicked();

    void on_btnClearOutput_clicked();

    void on_btnFoo_2_clicked();

    void on_radAsIs_toggled(bool checked);

    void on_tableUnOrdered_cellClicked(int row, int column);

    void on_pushButton_2_clicked();

public slots:
    void redBallClicked();
    void greenBallClicked();
    void blueBallClicked();
    void cyanBallClicked();
    void yellowBallClicked();
    void magentaBallClicked();

private:
    Ui::ColorSlider *ui;
    bool parseWholeFile(QString filename, int need_db_update);
    bool parseLastLineFile(QString filename, QVector<int> &vec, QDateTime &date, int value);
    void fill_table(QTableWidget *table, QVector<QVector<int> > &values);
    DataProcessing *proc[6];
//    DataProcessing *proc2;
//    DataProcessing *proc3;
//    DataProcessing *proc4;
//    DataProcessing *proc5;
//    DataProcessing *proc6;
    QList<QLabel*> labelNumbs1;
    QList<QLabel*> labelNumbs2;
    QList<QLabel*> labelNumbs3;
    QVector<int> columnsNumbers[6];
    QVector<QDate> date;
//    QVector<int> secondcol;
//    QVector<int> thirdcol;
//    QVector<int> fourthcol;
//    QVector<int> fifthcol;
//    QVector<int> sixthcol;
    int lastIndex;
    QPainterPath line;
    PastRaffle db;

protected:
    virtual void wheelEvent(QWheelEvent * event);
    virtual void paintEvent(QPaintEvent *event);
    virtual bool eventFilter(QObject* watched, QEvent* event );
};

#endif // COLORSLIDER_H
