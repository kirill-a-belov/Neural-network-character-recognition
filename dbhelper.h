#ifndef DBHELPER_H
#define DBHELPER_H

#include <QBitArray>
#include <QtSql>



class DbHelper  //По паттерну Singletone
{

public:
    static DbHelper * getInstance();
    void writeNeuronMemory(char memory[4096], char name);
    void readNeuronMemory(char memory[4096], char name);

private:
    DbHelper();
    ~DbHelper();

private:

    QSqlDatabase db;
    QSqlQuery  query;


};

#endif // DBHELPER_H
