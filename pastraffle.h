#ifndef PASTRAFFLE_H
#define PASTRAFFLE_H
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDateTime>

#define NUMS_APOSTADOS "NUMEROS_APOSTADOS"          //antigo NUMBERS
#define NUMS_ORDENADOS "NUMEROS_ORDEM_CRESCENTE"    //antigo NUMBERS_ORDERED
#define NUMS_SORTEADOS "NUMEROS_ORDEM_SORTEIO"      //antigo PASTRAFFLE

#define COLUNA_1 "firstColumn"
#define COLUNA_2 "secondColumn"
#define COLUNA_3 "thirdColumn"
#define COLUNA_4 "fourthColumn"
#define COLUNA_5 "fifthColumn"
#define COLUNA_6 "sixthColumn"


class PastRaffle
{
public:
    PastRaffle();
    void tableCreate(QString table);
    void insertNumbers(QString table, QVector<int> &nums, QDate raffletime);
    int pickAllNumbers(QString table, QVector<QVector<int> > &vec);
    int pickAllColumnLinesFromTable(QString table, QString columns, QVector<int> &vec);
    QVector<int> pickNumbers(QString table, int line);
    QDateTime pickDate(QString table, int line);
    int linesCount(QString table);
    int foo(QString table, QVector<int> &nums);
    int searchByDate(QString table, QVector<int> &nums, QDate time);
    int searchAll(QString table, QVector<int> &nums);

private:
    QSqlDatabase db;
};

#endif // PASTRAFFLE_H
