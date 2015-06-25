#ifndef CTHREAD_H
#define CTHREAD_H
#include "defStruct.h"
#include "CA.h"

class CThread
{
  protected:
    Figure_t listFigure;
    //������ �������� ������, ������������ ��� ������ �����
    CListCA *ptrOutputListCA;
    //����� ����
    unsigned long int step;
    //��������� �� ������ ������ �������� ����
    CListCA *ptrCurListCA;
    //��������� �� ������ ������ ������������ ����
    CListCA *ptrNextListCA;
    //������ ����� �����
    CListInt listInt;

  public:
    //���������� ������
    unsigned long int numCA;
    //��������� �� ������� ����� ������������ � ������
    CA *ptrCA;
    //������ ������ �� ������ ����
     void genGrid();
    CListCA listCA2;
    CListCA listCA1;
    //�������������
    virtual void init();
    //������ ���� �������������
    virtual void run();
    //��������� ������ ������ ������� ����� �����������������
    CListCA *getOutputListCA();
    Figure_t *getListFigure();
	virtual unsigned char getNumModeVisualisation();
};
#endif