#ifndef CTHREAD_CA1_H
#define CTHREAD_CA1_H
#include "CThread.h"
#include <implHard.h>

class CThreadImplHard:public CThread
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

  virtual void run();
  unsigned char getNumModeVisualisation();
};
#endif