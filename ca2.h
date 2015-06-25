#ifndef CA2_H
#define CA2_H
#include <CA.h>
#define PERIOD 100 
//просто пример класса

class ca2:public CA{
  public:
    unsigned char nj;
	unsigned char nv;
	unsigned char nj2;
	unsigned char nv2;
	double energy_int;

    ca2(uint param0, uint param1, uint param2, uint param3);
    CA *localRule(CA *ptrCA,CListInt  *listNumCA, const CListCA *listCA, unsigned long int step);
    void copy(CA *ca);
    uchar check(int _x, int _y);
    void add(CListInt *ptr,int _x, int _y);
    void geometry(CListInt *ptr);

//  void *getParam(uchar *str, uint size){return 0;}
    void init(uint param);
    CColor getColor();
	
   private:
		double GetG1(double energy_int);
		double GetG2(double energy_int);
		double GetEnergy_int(double T_e,double e_max);
		
		double read(double epsilon,FILE *fp);
		double k(int a, int j);
		
		unsigned char FuncMod(signed int number, unsigned int module);
		unsigned char getNumModeVisualisation(); 
};

#endif