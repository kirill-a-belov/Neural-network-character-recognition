#include "neuron.h"

Neuron::~Neuron()
{
    // Перед закрытием программы "сливаем" обновлённый опыт нейронов в БД
    DbHelper::getInstance()->writeNeuronMemory(this->memory, this->name);
    delete[] memory;
}



void Neuron::train(QBitArray bitInput)
{

    /* Ищем совпадения между текущей битовой картой и памятью,
     * если таковые есть - увеличиваем "вес" пикселя в памяти
     */
    for (int i =0; i < 4096; ++i)
    {
        if  (bitInput[i] == true)
        {
            if (this->memory[i] <=10)
            {
                this->memory[i]++;
            }
        }
    }

}

int Neuron::analyze(QBitArray bitInput)
{
    this->output = 0;
    /* Ищем совпадения между текущей битовой картой и памятью,
     * если таковые есть - увеличиваем выход нейрона
     */
    for (int i =0; i < 4096; ++i)
    {
        if  (bitInput[i] == true)
        {
            if (this->memory[i] > 0)
            {
                this->output+=memory[i];
            }
        }
    }

  return this->output;

}


