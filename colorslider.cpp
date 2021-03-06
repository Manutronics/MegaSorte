#include "colorslider.h"
#include "ui_colorslider.h"
#include "layoutoverlay.h"
#include <QMessageBox>
#include <QtMath>
#include <QFile>

ColorSlider::ColorSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorSlider)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);
//        setStyleSheet("background:transparent;");
//        setAttribute(Qt::WA_TranslucentBackground);
//        setWindowFlags(Qt::FramelessWindowHint);
        setWindowState(Qt::WindowMaximized);

    ui->horizontalSlider->setRange(0, 511);//color slider
    ui->label->setAutoFillBackground(true);
    ui->textOutput->setStyleSheet("QTextEdit { background-color: rgb(210, 210, 210) }");

    // create connection between axes and scroll bars:
    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(horzScrollBarChanged(int)));
    connect(ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    connect(ui->plot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));

    connect(ui->redLabel, SIGNAL(sendLabelClicked()), this, SLOT(redBallClicked()));
    connect(ui->greenLabel, SIGNAL(sendLabelClicked()), this, SLOT(greenBallClicked()));
    connect(ui->blueLabel, SIGNAL(sendLabelClicked()), this, SLOT(blueBallClicked()));
    connect(ui->cyanLabel, SIGNAL(sendLabelClicked()), this, SLOT(cyanBallClicked()));
    connect(ui->magentaLabel, SIGNAL(sendLabelClicked()), this, SLOT(magentaBallClicked()));
    connect(ui->yellowLabel, SIGNAL(sendLabelClicked()), this, SLOT(yellowBallClicked()));

    ui->plot->xAxis->axisRect()->setRangeDrag(Qt::Horizontal);
    ui->plot->xAxis->axisRect()->setRangeZoom(Qt::Horizontal);

    QVector<int> ultimo_sorteio;
    QDateTime dia;
    int ultimo_sorteio_id;

    ultimo_sorteio_id = db.linesCount(NUMS_SORTEADOS);
    qDebug() << "ultimo sorteio id -> " << ultimo_sorteio_id;

//    if(!parseLastLineFile("d_mega.htm", ultimo_sorteio, dia, ultimo_sorteio_id))
//        return;

    if(dia.daysTo(db.pickDate(NUMS_SORTEADOS, ultimo_sorteio_id))<-4)
    {
        qDebug() << "opa";
    }

    if(!parseWholeFile("d_mega.htm", ultimo_sorteio_id))
        return;

    lastIndex = columnsNumbers[FIRSTCOL].size()-1;
//    db.pickAllColumnLinesFromTable(NUMS_SORTEADOS, COLUNA_1, columnsNumbers[FIRSTCOL]);
//    db.pickAllColumnLinesFromTable(NUMS_SORTEADOS, COLUNA_2, columnsNumbers[SECONDCOL]);
//    db.pickAllColumnLinesFromTable(NUMS_SORTEADOS, COLUNA_3, columnsNumbers[THIRDCOL]);
//    db.pickAllColumnLinesFromTable(NUMS_SORTEADOS, COLUNA_4, columnsNumbers[FOURTHCOL]);
//    db.pickAllColumnLinesFromTable(NUMS_SORTEADOS, COLUNA_5, columnsNumbers[FIFTHCOL]);
//    db.pickAllColumnLinesFromTable(NUMS_SORTEADOS, COLUNA_6, columnsNumbers[SIXTHCOL]);
//    ultimo_sorteio_id = columnsNumbers[FIRSTCOL].size();

    createGridNumbers(&labelNumbs1, ui->wdgNumPad);
    ui->horizontalScrollBar->setMaximum((ultimo_sorteio_id-1140)*1000);
    ui->horizontalScrollBar->setRange(0, (ultimo_sorteio_id-1140)*1000);
    ui->horizontalScrollBar->setSingleStep(5);
//    ui->tabObject->hide();
    ui->tableUnOrdered->hide();

    ui->spnFrom->setEnabled(false);
    ui->spnTo->setMaximum(ultimo_sorteio_id);
    ui->spnTo->setValue(ultimo_sorteio_id);
    ui->lblTurn->setText(QString::number(ultimo_sorteio_id));
    ui->btnCalculate->setToolTip("Este bot??o executa calculos e preenche os graficosde quadrantes");
    ui->btnCalculate->setToolTipDuration(3000);
    ui->btn_automate->setToolTip("Este bot??o executa calculos e apresenta os calculos na janela Qt");
    ui->btn_automate->setToolTipDuration(3000);

    for(int i = FIRSTCOL; i < NUMCOLUMNS; i++)
        proc[i] = new DataProcessing(ultimo_sorteio_id, this);

    QVector<QVector<int>> values;
    db.pickAllNumbers(NUMS_SORTEADOS, values);
    fill_table(ui->tableUnOrdered, values);
    values.clear();
    db.pickAllNumbers(NUMS_ORDENADOS, values);
    fill_table(ui->tableOrdered, values);
    values.clear();
}

ColorSlider::~ColorSlider()
{
    if(proc)
        for(int i = FIRSTCOL; i < NUMCOLUMNS; i++)
            delete proc[i];
    delete ui;
}

bool ColorSlider::parseWholeFile(QString filename, int need_db_update)
{
    QFile file(filename);
    QStringList lines;
    QString line;
    int last_update = need_db_update;

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0, "error", file.errorString());
        return false;
    }

    QTextStream in(&file);
    int i = 0;

    while(!in.atEnd())
    {
        line += in.readLine();
        if(line.contains("</tr>"))
        {
            lines = line.split(QRegularExpression("\<tr.*?\>|\<\/tr\>"), QString::SkipEmptyParts);
//            ui->tableUnOrdered->setRowCount(ui->tableUnOrdered->rowCount() + 1);
            foreach(QString sort, lines)
            {

                QStringList fields = sort.split(QRegularExpression("\<td.*?\>|\<\/td\>|&nbsp|\\s+\ts+\ts+"), QString::SkipEmptyParts);
                int j = 0;

                foreach(QString str, fields)
                {
                    if(str.contains("\r"))
                        continue;
//                    QtableUnOrderedItem *lengthItem = new QtableUnOrderedItem(str);
//                    ui->tableUnOrdered->setItem(i, j, lengthItem);
                    if(j==1)
                        date.append(QDate::fromString(str, "dd/MM/yyyy"));
                    if(j==2)
                        columnsNumbers[FIRSTCOL].append(str.toInt());
                    if(j==3)
                        columnsNumbers[SECONDCOL].append(str.toInt());
                    if(j==4)
                        columnsNumbers[THIRDCOL].append(str.toInt());
                    if(j==5)
                        columnsNumbers[FOURTHCOL].append(str.toInt());
                    if(j==6)
                        columnsNumbers[FIFTHCOL].append(str.toInt());
                    if(j==7)
                        columnsNumbers[SIXTHCOL].append(str.toInt());
                    j++;
                }
                if(columnsNumbers[FIRSTCOL].size() > last_update)
                {
                    last_update++;
                    QVector<int> vec;
                    vec << columnsNumbers[FIRSTCOL].last() << columnsNumbers[SECONDCOL].last() << columnsNumbers[THIRDCOL].last()
                        << columnsNumbers[FOURTHCOL].last() << columnsNumbers[FIFTHCOL].last() << columnsNumbers[SIXTHCOL].last();
                    db.insertNumbers(NUMS_SORTEADOS, vec, date.last());
                    std::sort(vec.begin(), vec.end());
                    db.insertNumbers(NUMS_ORDENADOS, vec, date.last());
                }
            }
            i++;
            line.clear();
            lines.clear();
        }
    }
    file.close();
    return true;
}

bool ColorSlider::parseLastLineFile(QString filename, QVector<int> &vec, QDateTime &date, int value)
{
    QFile file(filename);
    QStringList columns;
    QString line;

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0, "error", file.errorString());
        return false;
    }

    QTextStream in(&file);

    if(!in.seek(file.size()-1000))
    {
        QMessageBox::information(0, "error", "seek to invalid position");
        return false;
    }

    while(!in.atEnd())
    {
        line += in.readLine();

        if(line.contains("</tr>"))
        {
            columns = line.split(QRegularExpression("\<tr.*?\>|\<\/tr\>"), QString::SkipEmptyParts);
            foreach(QString sort, columns)
            {

                QStringList fields = sort.split(QRegularExpression("\<td.*?\>|\<\/td\>|&nbsp|\\s+\ts+\ts+"), QString::SkipEmptyParts);
                int j = 0;
                bool valid = false;
                foreach(QString str, fields)
                {
                    if(str.contains("\r"))
                        continue;

                    if(j==0 && str.toInt()>=value)
                    {
                        valid = true;
                        qDebug() << "valor" << str << "position" << in.pos() << "size" << file.size();
                    }

                    if(valid)
                    {
                        if(j==1)
                            date = QDateTime::fromString(str+" 00:00:00", "dd/MM/yyyy HH:mm:ss");
                        if(j==2)
                            vec << str.toInt();
                        if(j==3)
                            vec << str.toInt();
                        if(j==4)
                            vec << str.toInt();
                        if(j==5)
                            vec << str.toInt();
                        if(j==6)
                            vec << str.toInt();
                        if(j==7)
                            vec << str.toInt();                        
                    }
                    j++;
                }
            }
            line.clear();
        }
    }
    file.close();
    return true;
}

void ColorSlider::fill_table(QTableWidget *table, QVector<QVector<int>> &values)
{
//    qDebug() << "Row Count" << table->rowCount() << values.size();
    for(int f = 0; f < values.size(); f++)
    {
        QTableWidgetItem *id = new QTableWidgetItem(QString::number(f+1));
        table->setRowCount(table->rowCount()+1);
        table->setItem(table->rowCount()-1 , 0, id);
        table->item(f, 0)->setData(Qt::TextAlignmentRole, Qt::AlignCenter);
        QTableWidgetItem *dateItem = new QTableWidgetItem(date.at(f).toString("dd/MM/yyyy"));
        table->setItem(table->rowCount()-1 , 1, dateItem);
        table->item(f, 1)->setData(Qt::TextAlignmentRole, Qt::AlignCenter);
//        qDebug() << "aqui veio" << table->rowCount() << values.size();
        for(int i = 0; i < NUMCOLUMNS; i++)
        {
//            qDebug() << QString::number(values.at(f).at(i));
            QTableWidgetItem *lengthItem = new QTableWidgetItem(QString::number(values.at(f).at(i)));
            table->setItem(table->rowCount()-1 , i+2, lengthItem);
            table->item(f, i+2)->setData(Qt::TextAlignmentRole, Qt::AlignCenter);
        }
    }
//    table->setRowCount(table->rowCount()-1);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setVisible(false);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setAlternatingRowColors(true);
    table->scrollToBottom();
}

void ColorSlider::on_horizontalSlider_sliderMoved(int position)
{
    QPalette palette;
    QColor color;
    int r, g, b;
    r = 255;
    g = (255 - (511 - position))>0?qFloor(256 - (255 - (511 - position))):255;
    b = (255 - (511 - position))<0?qFloor(511 - (position * 2)/2):0;
    color.setRgb(qRgb(r, g, b));
    palette.setColor(ui->label->backgroundRole(), color);
    ui->label->setPalette(palette);
}

void ColorSlider::setupPlot(QVector<int> &parx)
{
//    int i, j;
    // The following plot setup is mostly taken from the plot demos:
    ui->plot->clearGraphs();
    ui->plot->addGraph();

    ui->plot->graph()->setPen(QPen(Qt::blue));
    ui->plot->graph()->setLineStyle(QCPGraph::lsLine);
    ui->plot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::yellow, 5));
//    ui->plot->graph()->setBrush(QBrush(QColor(0, 0, 255, 0)));
    QVector<double> x(parx.size()), y(parx.size()), y2(parx.size());


    for (int i = 0; i < parx.size(); i++)
    {
        x.append(parx.at(i));
        y.append(i);
    }

    ui->plot->graph(0)->setData(y, x);
    ui->plot->axisRect()->setupFullAxesBox();
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->plot->yAxis->setRange(0, 5);
//    ui->plot->yAxis->rescale(true);
//    ui->plot->yAxis->setTickStep(1);
//    ui->plot->yAxis->setAutoTickCount(1);
    ui->plot->replot();
}

void ColorSlider::setupPlot()
{
    int sz = proc[FIRSTCOL]->column.quadrant.size();
    // The following plot setup is mostly taken from the plot demos:
    ui->plot->clearGraphs();
    QVector<double> x(sz), y(sz);
    for(int k = FIRSTCOL; k < NUMCOLUMNS; k++)
    {
        ui->plot->addGraph();
        ui->plot->graph()->setPen(QPen(Qt::blue));
        ui->plot->graph()->setLineStyle(QCPGraph::lsLine);
        ui->plot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::yellow, 5));
        for (int i = 0; i < sz; i++)
        {
            x.append(i);
            y.append(proc[k]->column.quadrant.at(i)+(k*5));
        }
        ui->plot->graph()->setData(x, y);
        x.clear();
        y.clear();
    }
    ui->plot->axisRect()->setupFullAxesBox();
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->plot->yAxis2->setVisible(true);
    ui->plot->yAxis->setRange(0, 30);
    ui->plot->yAxis2->setRange(0, 5);
//    ui->plot->yAxis->rescale(true);
    QVector<double> yTicks, y2Ticks;
    QVector<QString> yLabels, y2Labels;
    yLabels << " " << "1" << "2" << "3" << "4" << " " << "1" << "2" << "3" << "4" << " " << "1" << "2" << "3" << "4"
            << " " << "1" << "2" << "3" << "4" << " " << "1" << "2" << "3" << "4" << " " << "1" << "2" << "3" << "4" << " ";
//    y2Labels << QString::fromLatin1("Coluna") << QString::fromLatin1("Coluna") << QString::fromLatin1("Coluna")
//                << QString::fromLatin1("Coluna")<< QString::fromLatin1("Coluna") << QString::fromLatin1("Coluna");
    y2Labels << "Coluna" << "Coluna" << "Coluna"
                << "Coluna" << "Coluna" << "Coluna";
    yTicks << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15
           << 16 << 17 << 18 << 19 << 20 << 21 << 22 << 23 << 24 << 25 << 26 << 27 << 28 << 29 << 30;
    y2Ticks << 0 << 1 << 2 << 3 << 4 << 5;// << 6;
//    ui->plot->yAxis2->setAutoTicks(false);
//    ui->plot->yAxis2->setAutoTickLabels(false);
//    ui->plot->yAxis2->setTickVector(y2Ticks);
//    ui->plot->yAxis2->setTickVectorLabels(y2Labels);
//    ui->plot->yAxis->setAutoTicks(false);
//    ui->plot->yAxis->setAutoTickLabels(false);
//    ui->plot->yAxis->setTickVector(yTicks);
//    ui->plot->yAxis->setTickVectorLabels(yLabels);
    ui->plot->replot();
}

void ColorSlider::horzScrollBarChanged(int value)
{
//    if (qAbs(ui->plot->xAxis->range().center()-value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
//    {
//        ui->plot->xAxis->setRange(value/100, ui->plot->xAxis->range().size(), Qt::AlignCenter);
//        ui->plot->replot();
//    }
}

void ColorSlider::vertScrollBarChanged(int value)
{
//    if (qAbs(ui->plot->yAxis->range().center()+value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
//    {
//        ui->plot->yAxis->setRange(-value/100.0, ui->plot->yAxis->range().size(), Qt::AlignCenter);
//        ui->plot->replot();
//    }
}

void ColorSlider::xAxisChanged(QCPRange range)
{
//    ui->horizontalScrollBar->setValue(qRound(range.center()*100.0)); // adjust position of scroll bar slider
//    ui->horizontalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

void ColorSlider::yAxisChanged(QCPRange range)
{
//    ui->verticalScrollBar->setValue(qRound(-range.center()*100.0)); // adjust position of scroll bar slider
//    ui->verticalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}


void ColorSlider::on_btnSelect_clicked()
{
//    int sorteio = 1700;
//    int coluna = 2;
//    QtableUnOrderedItem *item1 = ui->tableUnOrdered->item(sorteio-1, coluna++);

//    setupPlot(firstcol);
    int column = ui->spnColumn->value()-1;
    colorizeSequence(columnsNumbers[column], ui->spnTo->value()-10, ui->spnTo->value(), column);
    QColor color;
    if(column == 0)
    {
        color.setRgbF(1, 0, 0, 0.5);
    }
    else if (column == 1)
    {
        color.setRgbF(0, 1, 0, 0.5);
    }
    else if (column == 2)
    {
        color.setRgbF(0, 0, 1, 0.5);
    }
    else if (column == 3)
    {
        color.setRgbF(0, 1, 1, 0.5);
    }
    else if (column == 4)
    {
        color.setRgbF(1, 1, 0, 0.5);
    }
    else if (column == 5)
    {
        color.setRgbF(1, 0, 1, 0.5);
    }

    LoadingOverlay *lo = new LoadingOverlay(ui->wdgNumPad, &line, &color);
    lo->setObjectName("mypath");
    emit ui->wdgNumPad->resize(this->size());
    int upto = ui->spnTo->value();
    for(int i = FIRSTCOL; i < NUMCOLUMNS; i++)
    {
        proc[i]->calcNumOccurrences(columnsNumbers[i], upto);
        proc[i]->calcIntervalsMean(columnsNumbers[i], upto, ui->chkIsMean->isChecked());
    }
//    proc2->calcNumOccurrences(secondcol, upto);
//    proc2->calcIntervalsMean(secondcol, upto);
//    proc3->calcNumOccurrences(thirdcol, upto);
//    proc3->calcIntervalsMean(thirdcol, upto);
//    proc4->calcNumOccurrences(fourthcol, upto);
//    proc4->calcIntervalsMean(fourthcol, upto);
//    proc5->calcNumOccurrences(fifethcol, upto);
//    proc5->calcIntervalsMean(fifethcol, upto);
//    proc6->calcNumOccurrences(sixthcol, upto);
//    proc6->calcIntervalsMean(sixthcol, upto);
//    ui->btnSelect->setText(QString("%1 %2").arg(proc1->column.mostvalue).arg(proc1->column.leastvalue));
//    QWidget *nova = new QWidget(ui->tableUnOrdered);
//    ui->tabObject->addTab(nova, "nova");
//    createGridNumbers(&labelNumbs3, nova);
//    for(int k = 0; k < 60; k++)
//    {
//        qDebug() << QString("ocorrencia de %1").arg(k+1, 2, 10,QChar('0'))
//                << QString(" foi %1").arg(proc[FIRSTCOL]->column.occurence[k], 2, 10, QChar('0'))
//                << QString(" foi %1").arg(proc[SECONDCOL]->column.occurence[k], 2, 10, QChar('0'))
//                << QString(" foi %1").arg(proc[THIRDCOL]->column.occurence[k], 2, 10, QChar('0'))
//                << QString(" foi %1").arg(proc[FOURTHCOL]->column.occurence[k], 2, 10, QChar('0'))
//                << QString(" foi %1").arg(proc[FIFTHCOL]->column.occurence[k], 2, 10, QChar('0'))
//                << QString(" foi %1").arg(proc[SIXTHCOL]->column.occurence[k], 2, 10, QChar('0'));
//    }
}

void ColorSlider::colorizeSequence(QVector<int> vect, int start, int finish, int column)
{
    QString border;
    switch(column)
    {
    case FIRSTCOL:
        border = "border: 2px solid red;";
        break;
    case SECONDCOL:
        border = "border: 2px solid lime;";
        break;
    case THIRDCOL:
        border = "border: 2px solid blue;";
        break;
    case FOURTHCOL:
        border = "border: 2px solid cyan;";
        break;
    case FIFTHCOL:
        border = "border: 2px solid yellow;";
        break;
    case SIXTHCOL:
        border = "border: 2px solid magenta;";
        break;
    default:
        return;
    }

    double percentage = (511 - (finish-start&0x1FF))/511.0;

    for(int i = start, cr = 4095; i < finish; i++, cr -=(4095/(finish-start)))
    {
        QString complement = QString("background: transparent;");
        labelNumbs1[vect[i]-1]->setStyleSheet(border+complement);
        if(i==start)
        {
            QPoint p = labelNumbs1[vect[i]-1]->pos();
            int woff = labelNumbs1[vect[i]-1]->width()/2;
            int hoff = labelNumbs1[vect[i]-1]->height()/2;
            p.setX(p.x()+woff);
            p.setY(p.y()+hoff);
            line.moveTo(p);
        }
        else
        {
            QPoint p = labelNumbs1[vect[i]-1]->pos();
            int woff = labelNumbs1[vect[i]-1]->width()/2;
            int hoff = labelNumbs1[vect[i]-1]->height()/2;
            p.setX(p.x()+woff);
            p.setY(p.y()+hoff);
            line.lineTo(p);
        }
    }
    update();
}

void ColorSlider::wheelEvent(QWheelEvent * event)
{
    int ind;
    ind = stepLine(event->angleDelta().ry());
    ui->tableOrdered->selectRow(ind);
    ui->tableUnOrdered->selectRow(ind);
}

int ColorSlider::stepLine(int step)
{
    QColor color;
    QPalette palette;
    bool visible = false;
    QList<QLabel*> *labelNum;
    int theindex = lastIndex;
    //static int index2= columnsNumbers[FIRSTCOL].size()-1, index4 = columnsNumbers[FIRSTCOL].size()-1;

    visible = true;
    labelNum = &labelNumbs1;


    color.setBlue(0xFF);
    color.setGreen(0xFF);
    color.setRed(0xFF);

    if(!ui->chkIsDrawable->isChecked()||!(ui->redLabel->isChecked()||ui->greenLabel->isChecked()||ui->blueLabel->isChecked()||
                                          ui->cyanLabel->isChecked()||ui->magentaLabel->isChecked()||ui->yellowLabel->isChecked()))
    {
        for(int i = FIRSTCOL; i < NUMCOLUMNS; i++)
        {
            palette.setColor((*labelNum)[columnsNumbers[i][theindex]-1]->backgroundRole(), color);
            (*labelNum)[columnsNumbers[i][theindex]-1]->setPalette(palette);
            (*labelNum)[columnsNumbers[i][theindex]-1]->setAutoFillBackground(true);
        }
    }
    //        palette.setColor((*labelNum)[secondcol[theindex]-1]->backgroundRole(), color);
    //        (*labelNum)[secondcol[theindex]-1]->setPalette(palette);
    //        palette.setColor((*labelNum)[thirdcol[theindex]-1]->backgroundRole(), color);
    //        (*labelNum)[thirdcol[theindex]-1]->setPalette(palette);
    //        palette.setColor((*labelNum)[fourthcol[theindex]-1]->backgroundRole(), color);
    //        (*labelNum)[fourthcol[theindex]-1]->setPalette(palette);
    //        palette.setColor((*labelNum)[fifethcol[theindex]-1]->backgroundRole(), color);
    //        (*labelNum)[fifethcol[theindex]-1]->setPalette(palette);
    //        palette.setColor((*labelNum)[sixthcol[theindex]-1]->backgroundRole(), color);
    //        (*labelNum)[sixthcol[theindex]-1]->setPalette(palette);

    if(step <0)
    {
        theindex++;
        if(theindex>(columnsNumbers[FIRSTCOL].count()-1))
            theindex = columnsNumbers[FIRSTCOL].count()-1;

        //            qDebug()<< "ifirstcol[theindex]" << firstcol[theindex] << theindex;
    }
    else
    {
        theindex--;
        if(theindex < 0)
            theindex = 0;
        //            qDebug()<< "efirstcol[theindex]" << firstcol[theindex] << theindex;
    }
    lastIndex = theindex;
    if(ui->redLabel->isChecked())
    {
        color.setBlue(0x00);
        color.setGreen(0x00);
        color.setRed(0xFF);
        palette.setColor((*labelNum)[columnsNumbers[FIRSTCOL][theindex]-1]->backgroundRole(), color);
        (*labelNum)[columnsNumbers[FIRSTCOL][theindex]-1]->setPalette(palette);
        ui->redLabel->setText(QString::number(columnsNumbers[FIRSTCOL][theindex]));
    }
    if(ui->greenLabel->isChecked())
    {
        color.setBlue(0x00);
        color.setGreen(0xFF);
        color.setRed(0x00);
        palette.setColor((*labelNum)[columnsNumbers[SECONDCOL][theindex]-1]->backgroundRole(), color);
        (*labelNum)[columnsNumbers[SECONDCOL][theindex]-1]->setPalette(palette);
        ui->greenLabel->setText(QString::number(columnsNumbers[SECONDCOL][theindex]));
    }
    if(ui->blueLabel->isChecked())
    {
        color.setBlue(0xFF);
        color.setGreen(0x00);
        color.setRed(0x00);
        palette.setColor((*labelNum)[columnsNumbers[THIRDCOL][theindex]-1]->backgroundRole(), color);
        (*labelNum)[columnsNumbers[THIRDCOL][theindex]-1]->setPalette(palette);
        ui->blueLabel->setText(QString::number(columnsNumbers[THIRDCOL][theindex]));
    }
    if(ui->cyanLabel->isChecked())
    {
        color.setBlue(0xFF);
        color.setGreen(0xFF);
        color.setRed(0x00);
        palette.setColor((*labelNum)[columnsNumbers[FOURTHCOL][theindex]-1]->backgroundRole(), color);
        (*labelNum)[columnsNumbers[FOURTHCOL][theindex]-1]->setPalette(palette);
        ui->cyanLabel->setText(QString::number(columnsNumbers[FOURTHCOL][theindex]));
    }
    if(ui->magentaLabel->isChecked())
    {
        color.setBlue(0xFF);
        color.setGreen(0x00);
        color.setRed(0xFF);
        palette.setColor((*labelNum)[columnsNumbers[FIFTHCOL][theindex]-1]->backgroundRole(), color);
        (*labelNum)[columnsNumbers[FIFTHCOL][theindex]-1]->setPalette(palette);
        ui->magentaLabel->setText(QString::number(columnsNumbers[FIFTHCOL][theindex]));
    }
    if(ui->yellowLabel->isChecked())
    {
        color.setBlue(0x00);
        color.setGreen(0xFF);
        color.setRed(0xFF);
        palette.setColor((*labelNum)[columnsNumbers[SIXTHCOL][theindex]-1]->backgroundRole(), color);
        (*labelNum)[columnsNumbers[SIXTHCOL][theindex]-1]->setPalette(palette);
        ui->yellowLabel->setText(QString::number(columnsNumbers[SIXTHCOL][theindex]));
    }
    ui->lblTurn->setText("Consurso n??mero: " + QString::number(theindex+1));
    return theindex;
}

int ColorSlider::stepAnyLine(int step)
{
    QColor color;
    QPalette palette;
    bool visible = false;
    QList<QLabel*> *labelNum; //the table of numbers 1 to 60

    visible = true;
    labelNum = &labelNumbs1;


    color.setBlue(0xFF);
    color.setGreen(0xFF);
    color.setRed(0xFF);

    if(!ui->chkIsDrawable->isChecked()||!(ui->redLabel->isChecked()||ui->greenLabel->isChecked()||ui->blueLabel->isChecked()||
                                          ui->cyanLabel->isChecked()||ui->magentaLabel->isChecked()||ui->yellowLabel->isChecked()))
    {
        for(int i = FIRSTCOL; i < NUMCOLUMNS; i++)
        {
            palette.setColor((*labelNum)[columnsNumbers[i][lastIndex]-1]->backgroundRole(), color);
            (*labelNum)[columnsNumbers[i][lastIndex]-1]->setPalette(palette);
            (*labelNum)[columnsNumbers[i][lastIndex]-1]->setAutoFillBackground(true);
        }
    }
    lastIndex = step;

    ui->spinBox->setValue(columnsNumbers[FIRSTCOL][lastIndex]);
    ui->spinBox_2->setValue(columnsNumbers[SECONDCOL][lastIndex]);
    ui->spinBox_3->setValue(columnsNumbers[THIRDCOL][lastIndex]);
    ui->spinBox_4->setValue(columnsNumbers[FOURTHCOL][lastIndex]);
    ui->spinBox_5->setValue(columnsNumbers[FIFTHCOL][lastIndex]);
    ui->spinBox_6->setValue(columnsNumbers[SIXTHCOL][lastIndex]);

    if(ui->redLabel->isChecked())
    {
        color.setBlue(0x00);
        color.setGreen(0x00);
        color.setRed(0xFF);
        palette.setColor((*labelNum)[columnsNumbers[FIRSTCOL][lastIndex]-1]->backgroundRole(), color);
        (*labelNum)[columnsNumbers[FIRSTCOL][lastIndex]-1]->setPalette(palette);
    }
    if(ui->greenLabel->isChecked())
    {
        color.setBlue(0x00);
        color.setGreen(0xFF);
        color.setRed(0x00);
        palette.setColor((*labelNum)[columnsNumbers[SECONDCOL][lastIndex]-1]->backgroundRole(), color);
        (*labelNum)[columnsNumbers[SECONDCOL][lastIndex]-1]->setPalette(palette);
    }
    if(ui->blueLabel->isChecked())
    {
        color.setBlue(0xFF);
        color.setGreen(0x00);
        color.setRed(0x00);
        palette.setColor((*labelNum)[columnsNumbers[THIRDCOL][lastIndex]-1]->backgroundRole(), color);
        (*labelNum)[columnsNumbers[THIRDCOL][lastIndex]-1]->setPalette(palette);
    }
    if(ui->cyanLabel->isChecked())
    {
        color.setBlue(0xFF);
        color.setGreen(0xFF);
        color.setRed(0x00);
        palette.setColor((*labelNum)[columnsNumbers[FOURTHCOL][lastIndex]-1]->backgroundRole(), color);
        (*labelNum)[columnsNumbers[FOURTHCOL][lastIndex]-1]->setPalette(palette);
    }
    if(ui->magentaLabel->isChecked())
    {
        color.setBlue(0xFF);
        color.setGreen(0x00);
        color.setRed(0xFF);
        palette.setColor((*labelNum)[columnsNumbers[FIFTHCOL][lastIndex]-1]->backgroundRole(), color);
        (*labelNum)[columnsNumbers[FIFTHCOL][lastIndex]-1]->setPalette(palette);
    }
    if(ui->yellowLabel->isChecked())
    {
        color.setBlue(0x00);
        color.setGreen(0xFF);
        color.setRed(0xFF);
        palette.setColor((*labelNum)[columnsNumbers[SIXTHCOL][lastIndex]-1]->backgroundRole(), color);
        (*labelNum)[columnsNumbers[SIXTHCOL][lastIndex]-1]->setPalette(palette);
    }
    ui->lblTurn->setText(QString::number(lastIndex+1));
    return lastIndex;
}

void ColorSlider::createGridNumbers(QList<QLabel *> *numbs, QWidget *tab)
{
    QPalette palette;
    QColor color;
    QGridLayout *gbl = new QGridLayout(this);
    for(int i = 1, p = 0; i < 7; i++)
    {
        for(int j = 1; j < 11;j++, p++)
        {
            QLabel *lblnum = new QLabel(QString("%1").arg(p+1, 2, 10,QChar('0')));
            (*numbs) << lblnum;
            gbl->addWidget(lblnum, i, j);
            (*numbs)[p]->setAutoFillBackground(true);
            color.setBlue(0xFF);
            color.setGreen(0xFF);
            color.setRed(0xFF);
            palette.setColor((*numbs)[p]->backgroundRole(), color);
            (*numbs)[p]->setPalette(palette);
            (*numbs)[p]->setAlignment(Qt::AlignCenter);
        }
    }
    gbl->setSpacing(23);
    color.setBlue(0xFF);
    color.setGreen(0x55);
    color.setRed(0xFF);
    for(int i = FIRSTCOL; i < NUMCOLUMNS; i++)
    {
        qDebug() << "oxente " << columnsNumbers[i].last();
        palette.setColor((*numbs)[columnsNumbers[i].last()-1]->backgroundRole(), color);
        (*numbs)[columnsNumbers[i].last()-1]->setPalette(palette);
    }
//    palette.setColor((*numbs)[secondcol.last()-1]->backgroundRole(), color);
//    (*numbs)[secondcol.last()-1]->setPalette(palette);
//    palette.setColor((*numbs)[thirdcol.last()-1]->backgroundRole(), color);
//    (*numbs)[thirdcol.last()-1]->setPalette(palette);
//    palette.setColor((*numbs)[fourthcol.last()-1]->backgroundRole(), color);
//    (*numbs)[fourthcol.last()-1]->setPalette(palette);
//    palette.setColor((*numbs)[fifethcol.last()-1]->backgroundRole(), color);
//    (*numbs)[fifethcol.last()-1]->setPalette(palette);
//    palette.setColor((*numbs)[sixthcol.last()-1]->backgroundRole(), color);
//    (*numbs)[sixthcol.last()-1]->setPalette(palette);
    tab->setLayout(gbl);
    tab->setMaximumHeight(320);
    tab->setMinimumHeight(320);
    tab->setMaximumWidth(640);
    tab->setMinimumWidth(640);
}

void ColorSlider::paintEvent(QPaintEvent *event)
{
//    LoadingOverlay *lo = ui->wdgNumPad->findChild<LoadingOverlay *>("mypath");
//    if(lo != NULL)
//    {
//        delete lo;
//        colorizeSequence(firstcol, 1700, 1711, 6);
//        LoadingOverlay *ll = new LoadingOverlay(ui->wdgNumPad, &line);
//        ll->setObjectName("mypath");
//        emit ui->wdgNumPad->resize(this->size());
//    }
}

void ColorSlider::on_pushButton_clicked()
{
    qDebug() << proc[FIRSTCOL]->genPass(proc[FIRSTCOL]->column.probablyValues, ui->spnQuadrant1->value());
    qDebug() << proc[SECONDCOL]->genPass(proc[SECONDCOL]->column.probablyValues, ui->spnQuadrant2->value());
    qDebug() << proc[THIRDCOL]->genPass(proc[THIRDCOL]->column.probablyValues, ui->spnQuadrant3->value());
    qDebug() << proc[FOURTHCOL]->genPass(proc[FOURTHCOL]->column.probablyValues, ui->spnQuadrant4->value());
    qDebug() << proc[FIFTHCOL]->genPass(proc[FIFTHCOL]->column.probablyValues, ui->spnQuadrant5->value());
    qDebug() << proc[SIXTHCOL]->genPass(proc[SIXTHCOL]->column.probablyValues, ui->spnQuadrant6->value());

//    proc[ui->spnColumn->value()]->showIntervalsOf(ui->spnColumn->value(), proc[SIXTHCOL]->column.probablyValues);
//    if(ui->spnColumn->value()==1)
//    {
//        proc[ui->spnColumn->value()-1]->findQuadrants(columnsNumbers[ui->spnColumn->value()-1]);
//        proc1->nextQuadrantEstatistical(proc1->column.quadrant.last());
//        setupPlot(proc[ui->spnColumn->value()-1]->column.quadrant);
//    }
//    else if(ui->spnColumn->value()==2)
//    {
//        proc[SECONDCOL]->findQuadrants(columnsNumbers[SECONDCOL]);
//        setupPlot(proc[SECONDCOL]->column.quadrant);
//    }
//    else if(ui->spnColumn->value()==3)
//    {
//        proc[THIRDCOL]->findQuadrants(columnsNumbers[THIRDCOL]);
//        setupPlot(proc[THIRDCOL]->column.quadrant);
//    }
//    else if(ui->spnColumn->value()==4)
//    {
//        proc[FOURTHCOL]->findQuadrants(columnsNumbers[FOURTHCOL]);
//        setupPlot(proc[FOURTHCOL]->column.quadrant);
//    }
//    else if(ui->spnColumn->value()==5)
//    {
//        proc[FIFTHCOL]->findQuadrants(columnsNumbers[FIFTHCOL]);
//        setupPlot(proc[FIFTHCOL]->column.quadrant);
//    }
//    else if(ui->spnColumn->value()==6)
//    {
//        proc[SIXTHCOL]->findQuadrants(columnsNumbers[SIXTHCOL]);
//        setupPlot(proc[SIXTHCOL]->column.quadrant);
//    }
}

void ColorSlider::on_btnCalculate_clicked()
{
    if(proc[0] == nullptr)
    {
        qDebug() << "ainda nao" << endl;
        return;
    }
    for(int i = FIRSTCOL; i < NUMCOLUMNS; i++)
    {
        proc[i]->allprocess = false;
        proc[i]->findQuadrants(columnsNumbers[i]);
        proc[i]->showIntervalsOf(ui->spnTo->value(), columnsNumbers[i]);
    }

    setupPlot();
}

void ColorSlider::on_btn_automate_clicked()
{
    QVector<int> gain;
    int rules = 0, matchfound = 0;
    //start the counter here
    for(int f = 1150; f < columnsNumbers[FIRSTCOL].count()-1; f++)
    {
        ui->spnTo->setValue(f); //first round with new rules

    //    generate the statistical here
        int upto = ui->spnTo->value();
        for(int h = FIRSTCOL; h < NUMCOLUMNS; h++)
        {
            proc[h]->allprocess = true;
            proc[h]->calcNumOccurrences(columnsNumbers[h], upto);
            proc[h]->calcIntervalsMean(columnsNumbers[h], upto, ui->chkIsMean->isChecked());
        }

        ui->spnTo->setValue(f+1);

        for(int i = FIRSTCOL; i < NUMCOLUMNS; i++)
        {
            proc[i]->findQuadrants(columnsNumbers[i]);
//            proc[i]->showIntervalsOf(ui->spnTo->value(), columnsNumbers[i]);
        }

        //process the numbers of the next level here
        for(int i = FIRSTCOL; i < NUMCOLUMNS; i++)
        {            
            for(int k = 0; k < proc[i]->column.probablyValues.count(); k++)
            {
//                qDebug() << "column" << i+1;
                if(proc[i]->column.probablyValues.at(k) ==  columnsNumbers[i].at(f))
                {
                    rules++;
//                    qDebug() << "oi" << proc[i]->column.probablyValues.at(k) <<  columnsNumbers[i].at(f) << "rules" << rules;
                    break;
                }
            }
            if(rules == 6)
            {
                if(i == 5)
                {
                    matchfound++;
                    gain << f+1;
                }
                rules = 0;
                break;
            }
        }
    }
    qDebug() << "resultado esperado-> " << matchfound << "  nos sorteios-> " << gain;
    //generate the next numbers compare with the real numbers
}

void ColorSlider::on_btnClearPanel_clicked()
{
    for(int k = 0; k < 59; k++)
        labelNumbs1[k]->setStyleSheet("");
}

void ColorSlider::on_btnSimulate_clicked()
{
//    QVector<int> gain;
//    int rules = 0, matchfound = 0;
//    //start the counter here
//    for(int f = 1150; f < columnsNumbers[FIRSTCOL].count()-1; f++)
//    {
//        ui->spnTo->setValue(f); //first round with new rules

//    //    generate the statistical here
//        int upto = ui->spnTo->value();
//        for(int h = FIRSTCOL; h < NUMCOLUMNS; h++)
//        {
//            proc[h]->allprocess = true;
//            proc[h]->calcNumOccurrences(columnsNumbers[h], upto);
//            proc[h]->calcIntervalsMean(columnsNumbers[h], upto, ui->chkIsMean->isChecked());
//        }

//        ui->spnTo->setValue(f+1);

////        for(int i = FIRSTCOL; i < NUMCOLUMNS; i++)
////        {
////            proc[i]->findQuadrants(columnsNumbers[i]);
////            proc[i]->showIntervalsOf(ui->spnTo->value(), columnsNumbers[i]);
////        }

//        //process the numbers of the next level here
//        int c1 = proc[FIRSTCOL]->genPass(proc[FIRSTCOL]->column.probablyValues, ui->spnQuadrant1->value());
//        int c2 = proc[SECONDCOL]->genPass(proc[SECONDCOL]->column.probablyValues, ui->spnQuadrant2->value());
//        int c3 = proc[THIRDCOL]->genPass(proc[THIRDCOL]->column.probablyValues, ui->spnQuadrant3->value());
//        int c4 = proc[FOURTHCOL]->genPass(proc[FOURTHCOL]->column.probablyValues, ui->spnQuadrant4->value());
//        int c5 = proc[FIFTHCOL]->genPass(proc[FIFTHCOL]->column.probablyValues, ui->spnQuadrant5->value());
//        int c6 = proc[SIXTHCOL]->genPass(proc[SIXTHCOL]->column.probablyValues, ui->spnQuadrant6->value());

//        for(int i = FIRSTCOL; i < NUMCOLUMNS; i++)
//        {
////            for(int k = 0; k < proc[i]->column.probablyValues.count(); k++)
////            {
////                qDebug() << "column" << i+1;
//                if((c1 ==  columnsNumbers[i].at(f))||(c2 ==  columnsNumbers[i].at(f))||(c3 ==  columnsNumbers[i].at(f))
//                        ||(c4 ==  columnsNumbers[i].at(f))||(c5 ==  columnsNumbers[i].at(f))||(c6 ==  columnsNumbers[i].at(f)))
//                {
//                    rules++;
////                    qDebug() << "oi" << proc[i]->column.probablyValues.at(k) <<  columnsNumbers[i].at(f) << "rules" << rules;
////                    break;
//                }
////            }
//            if(rules == 6)
//            {
//                if(i == 5)
//                {
//                    matchfound++;
//                    gain << f+1;
//                    qDebug() << " c1->" << c1 << " c2->" << c2 << " c3->" << c3 << " c4->" << c4 << " c5->" << c5 << " c6->" << c6;
//                }
//                rules = 0;
//                break;
//            }
//        }
//    }
//    qDebug() << "resultado esperado-> " << matchfound << "  nos sorteios-> " << gain;
//    //generate the next numbers compare with the real numbers
}

void ColorSlider::on_btnRegister_clicked()
{
    QVector<int> vec; //here is putted the numbers from first until sixth column
//    vec << ui->spinBox->value() << ui->spinBox_2->value() << ui->spinBox_3->value() << ui->spinBox_4->value() << ui->spinBox_5->value() << ui->spinBox_6->value();
//    std::sort(vec.begin(), vec.end());
//    db.insertNumbers("PASTRAFFLE", vec, QDateTime::currentDateTime());
//    vec.clear();
    for(int vline = 0; vline < columnsNumbers[1].size(); vline++)
    {
        vec = db.pickNumbers(NUMS_ORDENADOS, vline);
        for(int i = 0; i < NUMCOLUMNS; i++)
        {
            QTableWidgetItem *lengthItem = new QTableWidgetItem(QString::number(vec.at(i)));
            ui->tableOrdered->setItem(ui->tableOrdered->rowCount()-1 , i, lengthItem);
            ui->tableOrdered->item(ui->tableOrdered->rowCount()-1, i)->setData(Qt::TextAlignmentRole, Qt::AlignCenter);
        }
        ui->tableOrdered->setRowCount(ui->tableOrdered->rowCount()+1);
    }
}

//This click event putz all rounds of the raffles ordered
void ColorSlider::on_btnMake_clicked()
{
    QVector<int> values, notHide;
    QDate daVirada(2012, 12, 31);
    int rows = db.searchByDate(NUMS_SORTEADOS, values, daVirada);
    qDebug() << "line = " << rows << " numeros = " << values;

    for(int show = 0; show < (rows*7); show+=7)
    {
        ui->textOutput->append("<html>"
                               "<head>"
                               "<style>td.indent{ padding-left: 1.8em }</style>"
                               "</head>"
                               "<body>"
                               "<table style=\"width:100%\">"
                               "<tr>"
                               " <td>Sorteio n "+QString("%1").arg(values.at(show), 4, 10,QChar('0'))+"</td>"
                               " <td class = \"indent\"><font color = \"#FF0000\">"+QString("%1").arg(values.at(show+1), 2, 10,QChar('0'))+"</font></td>"
                               " <td class = \"indent\"><font color = \"#00AA10\">"+QString("%1").arg(values.at(show+2), 2, 10,QChar('0'))+"</font></td>"
                               " <td class = \"indent\"><font color = \"#0000FF\">"+QString("%1").arg(values.at(show+3), 2, 10,QChar('0'))+"</font></td>"
                               " <td class = \"indent\"><font color = \"#00FFFF\">"+QString("%1").arg(values.at(show+4), 2, 10,QChar('0'))+"</font></td>"
                               " <td class = \"indent\"><font color = \"#AA11AA\">"+QString("%1").arg(values.at(show+5), 2, 10,QChar('0'))+"</font></td>"
                               " <td class = \"indent\"><font color = \"#FFFF00\">"+QString("%1").arg(values.at(show+6), 2, 10,QChar('0'))+"</font></td>"
                               "</tr>"
                               "</table>"
                               "</body>"
                               "</html>");
        notHide << values.at(show);
    }
    notHide.append(0);
    for(int lss = 1, internal = 0; lss <= ui->tableOrdered->rowCount(); lss++)
    {
        ui->tableOrdered->showRow(lss);
        if(lss == notHide.at(internal))
        {
            internal++;
            continue;
        }
        ui->tableOrdered->hideRow(lss-1);
    }
}

void ColorSlider::on_btnFoo_clicked()
{
    int val;
    QVector<int> tudo, notHide;
    tudo << ui->spinBox->value() << ui->spinBox_2->value() << ui->spinBox_3->value() << ui->spinBox_4->value()
         << ui->spinBox_5->value() << ui->spinBox_6->value();
    if(ui->radAsIs->isChecked())
    {
        val = db.foo(NUMS_SORTEADOS, tudo);
    }
    else
    {
        val = db.foo(NUMS_ORDENADOS, tudo);
    }

    qDebug() << "line = " << val << " numeros = " << tudo;

    for(int show = 0; show < (val*7); show+=7)
    {
        ui->textOutput->append("<html>"
                               "<head>"
                               "<style>td.indent{ padding-left: 1.8em }</style>"
                               "</head>"
                               "<body>"
                               "<table style=\"width:100%\">"
                               "<tr>"
                               " <td>Sorteio n "+QString("%1").arg(tudo.at(show), 4, 10,QChar('0'))+"</td>"
                               " <td class = \"indent\"><font color = \"#FF0000\">"+QString("%1").arg(tudo.at(show+1), 2, 10,QChar('0'))+"</font></td>"
                               " <td class = \"indent\"><font color = \"#00AA10\">"+QString("%1").arg(tudo.at(show+2), 2, 10,QChar('0'))+"</font></td>"
                               " <td class = \"indent\"><font color = \"#0000FF\">"+QString("%1").arg(tudo.at(show+3), 2, 10,QChar('0'))+"</font></td>"
                               " <td class = \"indent\"><font color = \"#00FFFF\">"+QString("%1").arg(tudo.at(show+4), 2, 10,QChar('0'))+"</font></td>"
                               " <td class = \"indent\"><font color = \"#AA11AA\">"+QString("%1").arg(tudo.at(show+5), 2, 10,QChar('0'))+"</font></td>"
                               " <td class = \"indent\"><font color = \"#FFFF00\">"+QString("%1").arg(tudo.at(show+6), 2, 10,QChar('0'))+"</font></td>"
                               "</tr>"
                               "</table>"
                               "</body>"
                               "</html>");
        notHide << tudo.at(show);
    }
    notHide.append(0);
    for(int lss = 1, internal = 0; lss <= ui->tableOrdered->rowCount(); lss++)
    {
        ui->tableOrdered->showRow(lss);
        if(lss == notHide.at(internal))
        {
            internal++;
            continue;
        }
        ui->tableOrdered->hideRow(lss-1);
    }

//    int val, aux;
//    QVector<int> tudo;
//    val = db.linesCount(NUMS_ORDENADOS);
//    for(int l = 1; l <= val; l++)
//    {
//        tudo = db.pickNumbers(NUMS_ORDENADOS, l);
//        aux = db.foo(NUMS_ORDENADOS, tudo);
//        if(aux > 1)
//        {
//            qDebug() << "encontrou = " << aux;
//        }
//    }

//    int val;
//    val = db.linesCount(NUMS_SORTEADOS);
//    qDebug() << "numeros = " << val;
}

void ColorSlider::on_btnClearOutput_clicked()
{
    ui->textOutput->clear();
    for(int lss = 1; lss <= ui->tableOrdered->rowCount(); lss++)
    {
        ui->tableOrdered->showRow(lss);
    }
    ui->tableOrdered->scrollToTop();
    ui->tableOrdered->scrollToBottom();
}

void ColorSlider::on_btnFoo_2_clicked()
{
    int val;
    QVector<int> tudo, notHide;
    tudo << ui->spinBox->value() << ui->spinBox_2->value() << ui->spinBox_3->value() << ui->spinBox_4->value()
         << ui->spinBox_5->value() << ui->spinBox_6->value();
    if(ui->radAsIs->isChecked())
    {
        val = db.searchAll(NUMS_SORTEADOS, tudo);
    }
    else
    {
        val = db.searchAll(NUMS_ORDENADOS, tudo);
    }

    qDebug() << "line = " << val << " numeros = " << tudo;

    for(int show = 0; show < (val*7); show+=7)
    {
        ui->textOutput->append("<html>"
                               "<head>"
                               "<style>td.indent{ padding-left: 1.8em }</style>"
                               "</head>"
                               "<body>"
                               "<table style=\"width:100%\">"
                               "<tr>"
                               " <td>Sorteio n "+QString("%1").arg(tudo.at(show), 4, 10,QChar('0'))+"</td>"
                               " <td class = \"indent\"><font color = \"#FF0000\">"+QString("%1").arg(tudo.at(show+1), 2, 10,QChar('0'))+"</font></td>"
                               " <td class = \"indent\"><font color = \"#00FF00\">"+QString("%1").arg(tudo.at(show+2), 2, 10,QChar('0'))+"</font></td>"
                               " <td class = \"indent\"><font color = \"#0000FF\">"+QString("%1").arg(tudo.at(show+3), 2, 10,QChar('0'))+"</font></td>"
                               " <td class = \"indent\"><font color = \"#00FFFF\">"+QString("%1").arg(tudo.at(show+4), 2, 10,QChar('0'))+"</font></td>"
                               " <td class = \"indent\"><font color = \"#FF00FF\">"+QString("%1").arg(tudo.at(show+5), 2, 10,QChar('0'))+"</font></td>"
                               " <td class = \"indent\"><font color = \"#FFFF00\">"+QString("%1").arg(tudo.at(show+6), 2, 10,QChar('0'))+"</font></td>"
                               "</tr>"
                               "</table>"
                               "</body>"
                               "</html>");
        notHide << tudo.at(show);
    }
    notHide.append(0);
    for(int lss = 1, internal = 0; lss <= ui->tableOrdered->rowCount(); lss++)
    {
        ui->tableOrdered->showRow(lss);
        if(lss == notHide.at(internal))
        {
            internal++;
            continue;
        }
        ui->tableOrdered->hideRow(lss-1);
    }
}

void ColorSlider::on_radAsIs_toggled(bool checked)
{
    if(!checked)
    {
        ui->tableUnOrdered->hide();
        ui->tableOrdered->show();
    }
    else
    {
        ui->tableOrdered->hide();
        ui->tableUnOrdered->show();
    }
}

bool ColorSlider::eventFilter(QObject* watched, QEvent* event)
{
    switch( event->type() )
    {
    case QEvent::KeyPress:
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(keyEvent->key() == Qt::Key_Up)
        {
            if(watched->objectName().contains("tableOrdered") )
            {
                qDebug() << "event object up" << event << watched;
                stepLine(30);
            }
        }
        else if(keyEvent->key() == Qt::Key_Down)
        {
            if(watched->objectName().contains("tableOrdered") )
            {
                qDebug() << "event object down" << event << watched;
                stepLine(-30);
            }
        }
    }
        break;
    default:
        break;
    }

    return QWidget::eventFilter( watched, event );
}

void ColorSlider::on_tableUnOrdered_cellClicked(int row, int column)
{
    Q_UNUSED(column)
    stepAnyLine(row);
}

void ColorSlider::on_pushButton_2_clicked()
{
    ui->spinBox->setValue(0);
    ui->spinBox_2->setValue(0);
    ui->spinBox_3->setValue(0);
    ui->spinBox_4->setValue(0);
    ui->spinBox_5->setValue(0);
    ui->spinBox_6->setValue(0);
    ui->spinBox->setFocus();
    unsigned i;
    int recorderSize = 34;
    short recorderBuffer[68];
    for (i = 0; i < recorderSize; i += 2 * sizeof(short)) {
        recorderBuffer[i >> 2] = recorderBuffer[i >> 1];
    }
}

void ColorSlider::redBallClicked()
{
    qDebug() << "isso ?? red" << endl;
}

void ColorSlider::greenBallClicked()
{
    qDebug() << "isso ?? green" << endl;
}

void ColorSlider::blueBallClicked()
{
    qDebug() << "isso ?? blue" << endl;
}

void ColorSlider::cyanBallClicked()
{
    qDebug() << "isso ?? cyan" << endl;
}

void ColorSlider::yellowBallClicked()
{
    qDebug() << "isso ?? yellow" << endl;
}

void ColorSlider::magentaBallClicked()
{
    qDebug() << "isso ?? magenta" << endl;
}
