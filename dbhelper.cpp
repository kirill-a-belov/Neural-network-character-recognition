#include "dbhelper.h"


DbHelper::DbHelper() //Цепляемся к БД
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("neuronetwork.db3");
        if (!db.open())
        {
            qDebug() << "Ошибка пр открытие БД";
         }
    query = QSqlQuery(db);
 }

DbHelper::~DbHelper()
{

    db.close();
}

DbHelper *DbHelper::getInstance() //Служебный метод синглтона
{
    static DbHelper instance;
            return &instance;
}

// Пишем опыт нейрона в БД
void DbHelper::writeNeuronMemory(char memory[], char name)
{

    QString nameQuoted = "\"";
    nameQuoted.append(name);
    nameQuoted.append("\"");
    QByteArray buffer(memory,4096);
    query.prepare( "UPDATE neurons SET memory = :Memory WHERE name = "+nameQuoted+" ;");
    //query.bindValue( ":Memory", buffer, QSql::In | QSql::Binary );
    query.bindValue(":Memory", buffer);
    if (!query.exec()) {
           qDebug() << "Ошибка в запросе: Запись памяти нейрона в БД";
       }


}
//Извлекаем опыт нейрона из БД
void DbHelper::readNeuronMemory(char  memory[], char name)
{

    QString nameQuoted = "\"";
    nameQuoted.append(name);
    nameQuoted.append("\"");
    query.prepare( "SELECT memory FROM neurons WHERE name = "+nameQuoted+";");

    if (!query.exec()) {
           qDebug() << "Ошибка в запросе: Чтение памяти нейрона из БД";
        }
    query.first();
    QByteArray buffer = query.record().value(0).toByteArray();

      for (int i =0; i < 4096; ++i)
    {
        memory[i] = buffer[i];
    }

}


