#include "pastraffle.h"
#include <QDebug>

PastRaffle::PastRaffle()
{
    QString name("dataBase.sqlite");
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(name);
    if(db.open())
    {
        qDebug() << "Conectado a base de dados";
    }
    else
    {
        qDebug() << "Não conectado a base de dados";
        return;
    }
    tableCreate(NUMS_SORTEADOS);
    tableCreate(NUMS_ORDENADOS);
    tableCreate(NUMS_APOSTADOS);
}

void PastRaffle::tableCreate(QString table)
{
    QString enquiry;
    enquiry.append(QString("CREATE TABLE IF NOT EXISTS %1("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "firstColumn INTEGER NOT NULL,"
                 "secondColumn INTEGER NOT NULL,"
                 "thirdColumn INTEGER NOT NULL,"
                 "fourthColumn INTEGER NOT NULL,"
                 "fifthColumn INTEGER NOT NULL,"
                 "sixthColumn INTEGER NOT NULL,"
                 "DiaDoSorteio DATE NOT NULL"
                 ");").arg(table));
    QSqlQuery query;
    query.prepare(enquiry);

    if(query.exec())
    {
        qDebug() << "Tabela criada com sucesso!";
    }
    else
    {
        qDebug() << "A tabela rafflenumbs não existe e/ou não foi criada corretamente";
        qDebug() << "ERROR ->" << query.lastError();
    }
}

void PastRaffle::insertNumbers(QString table, QVector<int> &nums, QDate raffletime)
{
    QDate time = raffletime;
    QString enquiry;
    enquiry.append(QString("INSERT INTO %1("
                 "firstColumn,"
                 "secondColumn,"
                 "thirdColumn,"
                 "fourthColumn,"
                 "fifthColumn,"
                 "sixthColumn,"
                 "DiaDoSorteio)").arg(table));
    enquiry.append(QString("VALUES(%1, %2, %3, %4, %5, %6, '%7');").arg(nums[0])
            .arg(nums[1]).arg(nums[2]).arg(nums[3]).arg(nums[4]).arg(nums[5]).arg(time.toString("yyyy-MM-dd")));
    QSqlQuery insertQuery;
    insertQuery.prepare(enquiry);

    if(insertQuery.exec())
    {
        qDebug() << "Linha inserida com sucesso!";
    }
    else
    {
        qDebug() << "Não foi possivel inserir linha corretamente";
        qDebug() << "ERROR ->" << insertQuery.lastError();
    }
}

int PastRaffle::pickAllNumbers(QString table, QVector<QVector<int>> &vec)
{
    QString enquiry;
    enquiry.append(QString("SELECT * FROM %1").arg(table));

    QSqlQuery retrieveQuery;
    retrieveQuery.prepare(enquiry);

    if(retrieveQuery.exec())
    {
        qDebug() << "Consulta realizada com sucesso!";
    }
    else
    {
        qDebug() << "Não foi possivel consultar linha corretamente";
        qDebug() << "ERROR ->" << retrieveQuery.lastError();
    }

    QVector<int> temp;
    int rows = retrieveQuery.size();

    while(retrieveQuery.next())
    {
        temp.append(retrieveQuery.value(1).toInt());
        temp.append(retrieveQuery.value(2).toInt());
        temp.append(retrieveQuery.value(3).toInt());
        temp.append(retrieveQuery.value(4).toInt());
        temp.append(retrieveQuery.value(5).toInt());
        temp.append(retrieveQuery.value(6).toInt());
        vec.append(temp);
        temp.clear();
    }

    return rows;
}

int PastRaffle::pickAllColumnLinesFromTable(QString table, QString columns, QVector<int> &vec)
{
    QString enquiry;
    enquiry.append(QString("SELECT %1 FROM %2").arg(columns).arg(table));

    QSqlQuery retrieveQuery;
    retrieveQuery.prepare(enquiry);

    if(retrieveQuery.exec())
    {
        qDebug() << "Consulta realizada com sucesso!";
    }
    else
    {
        qDebug() << "Não foi possivel consultar linha corretamente";
        qDebug() << "ERROR ->" << retrieveQuery.lastError();
    }

    int rows = retrieveQuery.size();

    while(retrieveQuery.next())
    {
        vec.append(retrieveQuery.value(0).toInt());
    }

    return rows;
}

QVector<int> PastRaffle::pickNumbers(QString table, int line)
{
    QString enquiry;
    enquiry.append(QString("SELECT * FROM %1 WHERE id=%2").arg(table).arg(line));

    QSqlQuery retrieveQuery;
    retrieveQuery.prepare(enquiry);

    if(retrieveQuery.exec())
    {
        qDebug() << "Consulta unica realizada com sucesso!";
    }
    else
    {
        qDebug() << "Não foi possivel consultar linha corretamente";
        qDebug() << "ERROR ->" << retrieveQuery.lastError();
    }

    QVector<int> temp;
    if(retrieveQuery.first())
    {
        temp.append(retrieveQuery.value(1).toInt());
        temp.append(retrieveQuery.value(2).toInt());
        temp.append(retrieveQuery.value(3).toInt());
        temp.append(retrieveQuery.value(4).toInt());
        temp.append(retrieveQuery.value(5).toInt());
        temp.append(retrieveQuery.value(6).toInt());
    }

    return temp;
}

QDateTime PastRaffle::pickDate(QString table, int line)
{
    QString enquiry;
    QDateTime time;
    enquiry.append(QString("SELECT DiaDoSorteio FROM %1 WHERE id=%2").arg(table).arg(line));

    QSqlQuery retrieveQuery;
    retrieveQuery.prepare(enquiry);

    if(retrieveQuery.exec())
    {
        qDebug() << "Consulta unica realizada com sucesso!";
    }
    else
    {
        qDebug() << "Não foi possivel consultar linha corretamente";
        qDebug() << "ERROR ->" << retrieveQuery.lastError();
    }

    if(retrieveQuery.first())
    {
        time = retrieveQuery.value(0).toDateTime();
    }
    return time;
}

int PastRaffle::linesCount(QString table)
{
    QString enquiry;
    enquiry.append(QString("SELECT COUNT(*) FROM %1").arg(table));

    QSqlQuery retrieveQuery;
    retrieveQuery.prepare(enquiry);

    if(retrieveQuery.exec())
    {
        qDebug() << "Consulta unica realizada com sucesso!";
    }
    else
    {
        qDebug() << "Não foi possivel consultar linha corretamente";
        qDebug() << "ERROR ->" << retrieveQuery.lastError();
    }

    if(retrieveQuery.first())
    {
        return retrieveQuery.value(0).toInt();
    }
    else
    {
        return 0;
    }
}

int PastRaffle::foo(QString table, QVector<int> &nums)
{
    QString enquiry, mountstatement;
    QString columns[] = {" first", " second", " third", " fourth", " fifth", " sixth"};
    mountstatement = QString("SELECT * FROM %1 WHERE ").arg(table);
    for(int e = 0; e < 6; e++)
    {
        if(nums.at(e)!=0)
        {
            mountstatement += columns[e] + QString("Column=%1 AND ").arg(nums.at(e));
        }
    }
    mountstatement.resize(mountstatement.size()-4);
//    enquiry.append(QString("SELECT * FROM %1 WHERE firstColumn=%2 AND "
//                           "secondColumn=%3 AND "
//                           "thirdColumn=%4 AND "
//                           "fourthColumn=%5 AND "
//                           "fifthColumn=%6 AND "
//                           "sixthColumn=%7").arg(table).arg(nums.at(0)).arg(nums.at(1)).arg(nums.at(2)).arg(nums.at(3)).arg(nums.at(4)).arg(nums.at(5)));

    enquiry.append(mountstatement);
    QSqlQuery retrieveQuery;
    retrieveQuery.prepare(enquiry);

    if(retrieveQuery.exec())
    {
        nums.clear();
        qDebug() << "Consulta unica realizada com sucesso!";
    }
    else
    {
        qDebug() << "Não foi possivel consultar linha corretamente";
        qDebug() << "ERROR ->" << retrieveQuery.lastError() << enquiry;
    }

    int rows = 0;

    while(retrieveQuery.next())
    {
        rows++;        
        nums << retrieveQuery.value(0).toInt() << retrieveQuery.value(1).toInt() << retrieveQuery.value(2).toInt() << retrieveQuery.value(3).toInt() << retrieveQuery.value(4).toInt() << retrieveQuery.value(5).toInt() << retrieveQuery.value(6).toInt();
//        qDebug() << retrieveQuery.value(7).toDate();
    }

    return rows;
}

int PastRaffle::searchByDate(QString table, QVector<int> &nums, QDate time)
{
//    QString enquiry = QString("SELECT id,strftime('%d',DiaDoSorteio) FROM %1 WHERE strftime('%d',DiaDoSorteio) IN('12');").arg(table).arg(time.day());// AND DATEPART(month, DiaDoSorteio = '%3'))").arg(table).arg(time.date().day()).arg(time.date().month());
    QString enquiry = QString("SELECT * FROM %1 WHERE strftime('%d',DiaDoSorteio) IN('%2') AND strftime('%m',DiaDoSorteio) IN('%3');").arg(table).arg(time.day()).arg(time.month());//='2006-12-23';").arg(table);

    QSqlQuery retrieveQuery;
    retrieveQuery.prepare(enquiry);

    if(retrieveQuery.exec())
    {
        nums.clear();
        qDebug() << "Consulta unica realizada com sucesso!";
    }
    else
    {
        qDebug() << "Não foi possivel consultar linha corretamente";
        qDebug() << "ERROR ->" << retrieveQuery.lastError() << enquiry;
    }

    int rows = 0;

    while(retrieveQuery.next())
    {
        rows++;
        nums << retrieveQuery.value(0).toInt() << retrieveQuery.value(1).toInt() << retrieveQuery.value(2).toInt() << retrieveQuery.value(3).toInt() << retrieveQuery.value(4).toInt() << retrieveQuery.value(5).toInt() << retrieveQuery.value(6).toInt();
    }
    return rows;
}

int PastRaffle::searchAll(QString table, QVector<int> &nums)
{
    QString enquiry, mountstatement;
    QString columns[] = {" first", " second", " third", " fourth", " fifth", " sixth"};
    mountstatement = QString("SELECT * FROM %1 WHERE ").arg(table);
    for(int e = 0; e < 6; e++)
    {
        if(nums.at(e)!=0)
        {
            if(e > 0)
                mountstatement += " AND";
            for(int w = 0; w < 6; w++)
                mountstatement += columns[w] + QString("Column=%1 OR ").arg(nums.at(e));
            mountstatement.resize(mountstatement.size()-3);
//            mountstatement += QString("INTERSECT SELECT * FROM %1 WHERE ").arg(table);
        }
    }
//    mountstatement.resize(mountstatement.size()-53);
    qDebug() << mountstatement;
//    enquiry.append(QString("SELECT * FROM %1 WHERE firstColumn=%2 AND "
//                           "secondColumn=%3 AND "
//                           "thirdColumn=%4 AND "
//                           "fourthColumn=%5 AND "
//                           "fifthColumn=%6 AND "
//                           "sixthColumn=%7").arg(table).arg(nums.at(0)).arg(nums.at(1)).arg(nums.at(2)).arg(nums.at(3)).arg(nums.at(4)).arg(nums.at(5)));

    enquiry.append(mountstatement);
    QSqlQuery retrieveQuery;
    retrieveQuery.prepare(enquiry);

    if(retrieveQuery.exec())
    {
        nums.clear();
        qDebug() << "Consulta unica realizada com sucesso!";
    }
    else
    {
        qDebug() << "Não foi possivel consultar linha corretamente";
        qDebug() << "ERROR ->" << retrieveQuery.lastError() << enquiry;
    }

    int rows = 0;

    while(retrieveQuery.next())
    {
        rows++;
        nums << retrieveQuery.value(0).toInt() << retrieveQuery.value(1).toInt() << retrieveQuery.value(2).toInt() << retrieveQuery.value(3).toInt() << retrieveQuery.value(4).toInt() << retrieveQuery.value(5).toInt() << retrieveQuery.value(6).toInt();
    }

    return rows;
}
