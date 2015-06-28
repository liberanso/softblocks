#ifndef CA_H
#define CA_H
#include "defStruct.h"

//объявление непосредственно КА
class CA
{
protected:
    uint l;
    uint w;
    uint h;
    //открытые методы
public:

    //id клетки
    uint id;
    //тип автомата
    uint type;
    //метод инициализации клетки
    virtual void init(uint,int*,int);
    //локальные правила клетки
    virtual CA *localRule(CA *ptrCA,CListInt  *listNumCA, const CListCA *listCA, unsigned long int step);
    //геометрия список соседий
    virtual void geometry(CListInt *ptr);
    //получение параметра, будет использоваться в будущем
    //на данный момент не используеься
//    virtual void *getParam(uchar *str,uint size);
    //конструктор класса, задается id
    CA(uint param0, uint param1, uint param2, uint param3 );
    virtual void copy(CA *ca);
    //вернусть цевт клетки
    virtual CColor getColor();
};
#endif