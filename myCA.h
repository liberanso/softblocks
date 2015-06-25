#ifndef MY_CA_H
#include <CA.h>
//просто пример класса
class myCa:public CA{
  public:
    double state;
	double if7;
    myCa(uint param0, uint param1, uint param2, uint param3);
    CA *localRule(CA *ptrCA,CListInt  *listNumCA, const CListCA *listCA, unsigned long int step);
    void copy(CA *ca);
    uchar check(int _x, int _y);
    void add(CListInt *ptr,int _x, int _y);
    void geometry(CListInt *ptr);

//    void *getParam(uchar *str, uint size){return 0;}
    void init(uint param, uint def_num);
    CColor getColor();
};

#endif