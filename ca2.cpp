#include <ca2.h>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <CThreadImplHard.h>
#include "CThreadCA.h"


using namespace std;
extern CListCA *listHardImpl;
//просто пример класса
extern unsigned char numVisualisation;
//ходов первой модели до тяжелой
static const int T = 100;
// Подгоночный коэффициент
static const float F = 1.0;

//??
static const float  Eth = 2.0f;

ca2::ca2(uint param0, uint param1, uint param2, uint param3):CA(param0,param1,param2, param3){
  type = 0;
}
    
CA *ca2::localRule(CA *ptrCA,CListInt  *listNumCA, const CListCA *listCA, unsigned long int step){
	
  //преобразвание типов
  ca2 *inputCA = dynamic_cast<ca2 *>(ptrCA);
  if(inputCA!=0)
  {
    *inputCA = *this;
	
	//количество ячеек
	//int  size=inputCA->size();
	
	//первый такт
	int T_e=0;
	double e_max=0.0;
	if(step%3==0)
	{		
	/*	for(int j=0; j<size; ++j)
		{
			implHard *ptrImplHard = dynamic_cast<implHard *>(listHardImpl->at(j));
			if(ptrImplHard!=0)
			{
				if(ptrImplHard->e > Eth)
				{	  
					T_e++;
				}
				if(ptrImplHard->e > e_max)
				{	  
					e_max=ptrImplHard->e;
				}
				
			}
		}
		energy_int=GetEnergy_int(T_e,e_max);
		
		nj=inputCA->nj+GetG1(energy_int)*energy_int;
		nv=inputCA->nv+GetG1(energy_int)*energy_int;
		
		nj2=inputCA->nj2+GetG2(energy_int)*energy_int;
		nv2=inputCA->nv2+GetG2(energy_int)*energy_int;*/
	}
	//второй такт
	else if(step%3==1)
	{
		/*nj=inputCA->nj-2*k(a,j)*pow(inputCA->nj,2);
		nv=inputCA->nv-2*k(a,v)*pow(inputCA->nv,2);
		
		nj2=inputCA->nj2+k(a,j)*pow(inputCA->nj2,2);
		nv2=inputCA->nv2+k(a,v)*pow(inputCA->nv2,2);*/
	}
	//второй такт
	else if(step%3==2)
	{
		int  ssize=listNumCA->size();
		if(ssize==7)
		{
			//поворот
		}
	}
  }

  return ptrCA;
}

void ca2::copy(CA *ca){
  ca2 *Ca = dynamic_cast<ca2 *>(ca);
  if(Ca != 0){
    *this =*Ca;
  }
}

uchar ca2::check(int _x, int _y){
  if(id == 1){}
  if(id == (_x*l+_y)){
    return 1;
  }
  else if(id == (_x*l+_y-1)){
    return 1;
  }
  else if(id == (_x*l+_y+1)){
    return 1;
  }
  else if(id == ((_x+1)*(l)) + _y){
    return 1;
  }
  else if(id == ((_x+1)*l+_y+1)){
    return 1;
  }
  else if(id == ((_x-1)*(l)) + _y){
    return 1;
  }
  else if(id == ((_x-1)*l+_y+1)){
    return 1;
  }
  return 0;
}

void ca2::add(CListInt *ptr,int _x, int _y){
  ptr->push_back(_y*w+_x);
  if(_y+1<w){
    ptr->push_back(((_y+1)*w+_x));
    if(_x+1>0){
      ptr->push_back(((_y+1)*w+_x+1));
    }
  }
  if(_y-1>0){
    ptr->push_back(((_y-1)*w+_x));
    if(_x+1>0){
      ptr->push_back(((_y-1)*w+_x+1));
    }
  }
  if(_x-1>0){
    ptr->push_back(((_y)*w+_x-1));
  }
  if(_x+1>0){
    ptr->push_back(((_y)*w+_x+1));
  }
}

void ca2::geometry(CListInt *ptr){
  //коэфициенты найден практическим способом
  int increment = 14;
  int decrement = 3;
  //начальная точка 0, 0
  int valueBegin = 0;
  int _x = 0;
  int _y = 0;
  int beginX = 0;
  //ptr->push_back(1);
  //fprintf(file, "__ID = %i\n",id);
  for(int i=-3; i<((int)w+3); ++i){
    //ищем центры, 0, 0 начальный центр
    _x = beginX;
    while(_x<((int)w)){
      if(check(i,_x)){
        add(ptr,i,_x);
      }
      else{
        _x += increment;
      }
    }
    beginX -= 3;
  }
}


//    void *getParam(uchar *str, uint size){return 0;}
void ca2::init(uint param){
    nj=0;
	nv=0;
	nj2=0;
	nv2=0;
	energy_int=0;
}
    
CColor ca2::getColor(){
  CColor color;
  color.red = 0;
  color.green = 0;
  color.blue = 0;
  
  if(numVisualisation == 0)
	{
	  if(nv>0)
	  {
		color.blue = 200;
	  }else if(nv>0)
	  {
		color.red = 200;
	  } else  if(nj2>0)
	  {
		color.green = 200;
	  }else  if(nv2>0)
	  {
		color.blue = 200;
		color.green = 200;
		color.red = 200;
	  }
	}
  return color;
  
}

// файл типа  text tab text
double ca2::read(double epsilon,FILE *fp){

	if(fp != NULL) 
	{ 
		int i=0; 
		char ch; 
		while((ch = getc(fp)) != EOF || i>=0) 
		{
			if(ch==epsilon){ return getc(fp); break;}
		}		
	} 
	else
	return 0;

	fclose(fp);
}

double ca2::GetEnergy_int(double T_e,double e_max)
{
  return e_max*T_e/T*F;
}

double ca2::GetG1(double epsilon){
	FILE *g1=fopen("g1.txt""","r");
	return read(epsilon,g1);
}

double ca2::GetG2(double epsilon){
	FILE *g2=fopen("g2.txt""","r");
	return read(epsilon,g2);
}

double k(int a, int j){
return 1;
}
