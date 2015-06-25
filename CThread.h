#ifndef CTHREAD_H
#define CTHREAD_H
#include "defStruct.h"
#include "CA.h"

class CThread
{
  protected:
    Figure_t listFigure;
    //список выходных клеток, используется для вывода цвета
    CListCA *ptrOutputListCA;
    //номер хода
    unsigned long int step;
    //указатель на список клеток текущего хода
    CListCA *ptrCurListCA;
    //указатель на список клеток последующего хода
    CListCA *ptrNextListCA;
    //список целых чисел
    CListInt listInt;

  public:
    //количество клеток
    unsigned long int numCA;
    //временная КА которая будет учавствовать в обмене
    CA *ptrCA;
    //список клеток на первом ходу
     void genGrid();
    CListCA listCA2;
    CListCA listCA1;
    //инициализация
    virtual void init();
    //Запуск хода моделирования
    virtual void run();
    //возращает список клеток которые будут визуализироваться
    CListCA *getOutputListCA();
    Figure_t *getListFigure();
	virtual unsigned char getNumModeVisualisation();
};
#endif