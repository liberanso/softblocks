#include <vector>
#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <algorithm>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "defStruct.h"
#include <CA.h>
#include <CThread.h>
#include <CThreadCA.h>
#include <CThreadImplHard.h>
#include "stdio.h"

unsigned char numVisualisation = 0;

//будет генерироваться имена типов классов
enum
{
    CA_myCa,
    CA_caLife
};



CListThread listThread;
unsigned long int numThread = 0;

extern "C" __declspec(dllexport) Figure_t *getVectorFigure(uchar num)
{
    if(num<listThread.size())
    {
        return listThread.at(numThread)->getListFigure();
    }
    else
    {
        return 0;
    }
}

extern "C" __declspec(dllexport) CListCA *getPtrListCA()
{
    CListCA *listCA = listThread.at(numThread)->getOutputListCA();
    return listCA;
}

//функция создания нового автомата
extern "C" __declspec(dllexport) void creareAutomate()
{
    srand(time(NULL));
    srand ( unsigned (time(0)));
    //очищаем поток
    listThread.clear();
	CThreadCA *ptrN = new CThreadCA;
    //создаем потоки необходимых автоматов
    listThread.push_back(new CThreadCA);

    for(uint i=0; i<listThread.size(); ++i)
    {
        listThread.at(i)->init();
    }
}

//функция перехода к следующему ходу
extern "C" __declspec(dllexport) void StepForwardAutomate()
{


    static uint step = 0;
    if(step++ == 3000)
	{ 
		step=0;
      for(uint i=0; i<listThread.size(); ++i)
      {		
        listThread.at(i)->init();
      }
    }
	
    for(uint i=0; i<listThread.size(); ++i)  
	{
        listThread.at(i)->run();
    }
}

//функция освобождения всех автоматов, клеток, высвобождение памяти
extern "C" __declspec(dllexport) void DestructAutomate(void* array)
{
    //тут надо будет правильно освобождать всю выделенную память

}


extern "C" __declspec(dllexport) unsigned char SetModeVisualization(unsigned long int i)
{
    numThread = i;
    return listThread.at(numThread)->getNumModeVisualisation();
}


extern "C" __declspec(dllexport) unsigned long int GetMaxModeVisualization()
{
    return 1;
}

extern "C" __declspec(dllexport) void setNumVisualisation(unsigned char index){
  numVisualisation = index;
}

