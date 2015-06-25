#ifndef CTHREAD_CA1_H
#define CTHREAD_CA1_H
#include "CThread.h"
#include <implHard.h>

class CThreadImplHard:public CThread
{
  //���������� ����������
  void genSquare(float x, float y,Coordinates_t *ptr);
  //���������� �������������� ������

  //���������� �����
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