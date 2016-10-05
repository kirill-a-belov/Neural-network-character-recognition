#ifndef NEURON_H
#define NEURON_H

#include <QBitArray>
#include <dbhelper.h>


class Neuron
{
public:
    ~Neuron();
    inline void init(char name); // Первичная инициализация нейронов
    void train(QBitArray bitInput); // Пишем (обновляем) опыт нейрона в базу
    int analyze(QBitArray bitInput); // Анализ битовой карты и вывод решения нейрона



public:
    char name;// Тут название нейрона – буква, с которой он ассоциируется
    QBitArray input; // Тут входной массив 64x64 (4096)
    int output;// Сюда он будет говорить, что решил
    char memory[4096]; // Тут он будет хранить опыт о предыдущем опыте


};

// Определяем вложенные функции

inline void Neuron::init(char name)
{
    //Инициализируем нейрон
    this->name = name;
    this->input.fill(false);
    this->output = 0;
    // Заполняем опыт нейрона из базы при создании экземпляра

   DbHelper::getInstance()->readNeuronMemory(this->memory, name);



}

#endif // NEURON_H
