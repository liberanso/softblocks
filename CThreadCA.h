#ifndef CTHREAD_CA_H
#define CTHREAD_CA_H
#include "CThread.h"

class CThreadCA:public CThread
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
  uint hasDefect; //if block has defect is true
  uint complBlocks; //���������� ������ ������
  uint uncomplBlocks; //���������� �������� ������ 
  void init();
unsigned char getNumModeVisualisation(); 
  //��������� id
  bool addId(CListInt *ptr,signed long int x, signed long int y);
  signed long int abs(signed long int x);

  //�������� ������ id
  CListInt getId(signed long int x, signed long int y);
  virtual bool run();
};
#endif