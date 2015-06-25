#ifndef IMPL_HARD_H
#define IMPL_HARD_H
#include <CA.h>
#include <settingImplHard.h>
unsigned char getNumModeVisualisation(); 
class implHard;
struct SI{
  implHard* ptr_automate;
  unsigned char i;
};
typedef vector<SI> CListI;
typedef vector<implHard*> CListImplHard;
typedef vector<int> CListEpsiloni;
typedef vector<unsigned char> ListUnsignedChar;

//просто пример класса
class implHard:public CA{
public:
  //state
  unsigned char p;
  unsigned char e;
  unsigned char epsilon;
  unsigned char v;
  signed char op;
  float ver;
  unsigned int ind;
  float ugl;   
  
  implHard(uint param0, uint param1, uint param2, uint param3);
  CA *localRule(CA *ptrCA,CListInt  *listNumCA, const CListCA *listCA, unsigned long int step);
  void copy(CA *ca);
  uchar check(int _x, int _y);
  void add(CListInt *ptr,int _x, int _y);
  void geometry(CListInt *ptr);
//    void *getParam(uchar *str, uint size){return 0;}
  void init(uint param);
  CColor getColor();
  
private:
    CListI GetI1(CListImplHard np);
	double GetNewGlobalW(double energy);
	double GetNewGlobalOmega(double energy);
	double GetNewGlobalAlpha(double energy);
	double GetNewGlobalBeta(double energy);
	double GetGamma();
	/*
	unsigned char Optimize(CListName_JDX np);
	*/
	CListI GetI4(CListI I);
	double GetNewGlobalAlphaSh(double energy);
	float S(float fi);
	float P(int fi1,int energy);
	float Pk(float fi1,float fi2,float energy);
	float Delta(float fi1,float fi2,float energy);
	CListI GetI2(CListI I);
	unsigned char GetDeltai(unsigned char vi,unsigned char i);
	unsigned char BitEpsilon(unsigned char eps);
	double GetNewGlobalBetaSh(double energy);
	unsigned char GetNextP(CListI I1, CListI I2,CListI I3);
	unsigned char GetNextEpsilon(CListImplHard np,CListI I2,CListI I3);
	unsigned char FuncMod(signed int number, unsigned int module);
	CListI GetI3(CListI I);
	unsigned char GetNextV(CListI I2,CListI I3,CListI I4);
	bool BitDropEnergy();
	unsigned char GetNextE(CListI I1, CListI I2,CListI I3);
	CListEpsiloni GetEpsiloni(CListImplHard np);
	signed char GetOp();
	signed char GetEps();
};

#endif