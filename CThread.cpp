#include "CThread.h"

void CThread::init(){
  listFigure.clear();
  step = 0;
}
void CThread::genGrid(){}

//������ ���� �������������
void CThread::run(){
  if((step&1) == 0){
    ptrCurListCA = &listCA1;
    ptrNextListCA = &listCA2;
  }
  else{
    ptrCurListCA = &listCA2;
    ptrNextListCA = &listCA1;
  }
  for(unsigned long int i=0; i<ptrCurListCA->size(); ++i){
    listInt.clear();
    //�������� ������ ������� ��� ������ ������
    ptrCurListCA->at(i)->geometry(&listInt);

    (ptrNextListCA->at(i)->copy((ptrCurListCA->at(i)->localRule(ptrCA,&listInt,ptrCurListCA,step))));
  }
  ptrOutputListCA = ptrNextListCA;
  ++step;
}
//��������� ������ ������ ������� ����� �����������������
CListCA *CThread::getOutputListCA(){
  return ptrOutputListCA;
}
Figure_t *CThread::getListFigure(){
  return &listFigure;
}

unsigned char CThread::getNumModeVisualisation(){
  return 2;
}