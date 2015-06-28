#ifndef CTHREAD_CA2_H
#define CTHREAD_CA2_H
#include "CThread.h"

class CThreadCA2:public CThread
{
  //генерируем координаты
  void genSquare(float x, float y,Coordinates_t *ptr);
  //генерируем гексогональную ячейку

  //генерируем сетку
  void genGrid();

public:
  uint l;
  uint w;
  uint h;
  void init();

  //добавляем id
  void addId(CListInt *ptr,signed long int x, signed long int y);
  signed long int abs(signed long int x);

  //получить список id
CListInt getId(signed long int x, signed long int y,char mode);

  virtual void run();
unsigned char getNumModeVisualisation();
};
#endif