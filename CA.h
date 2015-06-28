#ifndef CA_H
#define CA_H
#include "defStruct.h"

//���������� ��������������� ��
class CA
{
protected:
    uint l;
    uint w;
    uint h;
    //�������� ������
public:

    //id ������
    uint id;
    //��� ��������
    uint type;
    //����� ������������� ������
    virtual void init(uint,int*,int);
    //��������� ������� ������
    virtual CA *localRule(CA *ptrCA,CListInt  *listNumCA, const CListCA *listCA, unsigned long int step);
    //��������� ������ �������
    virtual void geometry(CListInt *ptr);
    //��������� ���������, ����� �������������� � �������
    //�� ������ ������ �� ������������
//    virtual void *getParam(uchar *str,uint size);
    //����������� ������, �������� id
    CA(uint param0, uint param1, uint param2, uint param3 );
    virtual void copy(CA *ca);
    //�������� ���� ������
    virtual CColor getColor();
};
#endif