#ifndef CTHREAD_CA_H
#define CTHREAD_CA_H
#include "CThread.h"

class CThreadCA:public CThread
{
  //генерируем координаты
  void genSquare(float x, float y,Coordinates_t *ptr);
  //генерируем гексагональную ячейку

  //генерируем сетку
  void genGrid();

public:
  uint l;
  uint w;
  uint h;
  uint hasDefect; //if block has defect is true
  uint complBlocks; //количество полных блоков
  uint uncomplBlocks; //количество неполных блоков 
  void init();
unsigned char getNumModeVisualisation(); 
  //добавляем id
  bool addId(CListInt *ptr,signed long int x, signed long int y);
  signed long int abs(signed long int x);

  //получить список id
  CListInt getId(signed long int x, signed long int y);
  virtual bool run();
};
#endif