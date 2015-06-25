#include <implHard.h>
#include <vector>
#include <math.h>
#include <stdio.h>
using namespace std;

extern FILE *history;
extern unsigned char numVisualisation;
extern ClistFloat listF[max_col_excel];
extern ClistFloat listP,listE,listFi,listE2,listT[LENGTH_ROW_T2];
//массив со случайным положением частиц, служит, чтобы потом сгенерировать появляющиеся частицы
extern unsigned char nl[MAX_PARTICULAR];                       
extern unsigned int p_collum[WIDTH];
extern unsigned int p_row[LENGTH];


static const double gamma = 0.001f;
static const float  Eth = 2.0f;
static const float M1 = 31;
static const float M2 = 28;

static double E0 = 100.0f/255.0f;
static double L = 2;
static double Pmax = 1.36f;

static const unsigned int P_MAX = 10000;                                       //используется в генерации сильных и слабых взаимодействий(делится на это число, чтобы получить дробное число)
static unsigned long init[4]={0x123, 0x234, 0x345, 0x456}, length=4;

float  W;                                               
float  Omega;                                           
double alpha;
double alpha_sh;
double beta;
double beta_sh;

unsigned long int GetId(int frow, int fcollum,bool *bit ){
    if((frow<0)||(frow>=WIDTH)){
      *bit = false;
      return 0xff;
    }
    if((fcollum<0)||(fcollum>=LENGTH)){
       *bit = false;
	return 0xff;
    }
    *bit = true;
    return frow*LENGTH + fcollum;
}


CListI implHard::GetI1(CListImplHard np){
  CListI I1;
  SI I;
  I1.clear();
  for(unsigned char i=0;i<np.size();++i){
    if(np.at(i)!=0){
      if((np.at(i)->e)>=Eth){
	    I.ptr_automate = np.at(i);
	    I.i = i; 
        I1.push_back(I);
      }
    }
  }
  return I1;
}


double implHard::GetNewGlobalW(double energy){
 double EE0 = e*E0;
  if(e == 0){return 0;}
  return 0.3*0.22f/Pmax*pow(EE0,-0.58);
}


double implHard::GetNewGlobalOmega(double energy){
  double EE0 = e*E0;
  if(e == 0){return 0;}
  return 1.0f/Pmax*(1.16f*pow(EE0,-0.4f) - 0.22f*pow(EE0,-0.58));
} 



double implHard::GetNewGlobalAlpha(double energy){
  double pi = 3.14f;
  double phi1 = 0;
  double phi2 = pi*30/180;
  double phi3 = pi*90/180;
  double result = GetGamma() + Delta(45,60,energy);/*+ (phi2/2 - sin(phi2/2)) - (phi1/2 - sin(phi1/2))*/;

  result = 1 - exp(-result);
  if(result<0){result = 0;}
  if(result >1){result = 1;}
  return result;
}


double implHard::GetNewGlobalBeta(double energy){
  double pi = 3.14f;
  double phi1 = 0;
  double phi2 = pi*30/180;
  double phi3 = pi*90/180;
  double result = GetGamma() + Delta(60,77,energy);
  result = 1-exp(-result);
  if(result<0){result = 0;}
  if(result >1){result = 1;}
  return result;
}

double implHard::GetGamma(){
  if(e==0){return 0;}
  double EE0 = e*E0;
  double temp;
  if(e<(12/E0)){
    temp = 0.277*pow(EE0,-0.79);
  }
  else{
    temp = -0.001415+0.466/EE0;
  }
  return (0.02833f*L/pow(EE0,0.5))+L*temp;
}

/*
unsigned char implHard::Optimize(CListName_JDX np){
  if(e!=0){return 0;}
  if(p!=0){return 0;}
  if(v!=0){return 0;}
  for(unsigned char i=0;i<np.size();++i){
    if(np.at(i)!=0){
      if((np.at(i)->e)!=0){return 0;}
      if((np.at(i)->p)!=0){return 0;}
      if((np.at(i)->v)!=0){return 0;}
    }
  }
  return 1;
}
*/
CListI implHard::GetI4(CListI I){
  CListI I1;
  I1.clear();
  
  for(unsigned char i=0;i<I.size();++i){
    signed char op_temp = I.at(i).ptr_automate->op;
    signed char t1 = (signed char)I.at(i).ptr_automate->v-(signed char)I.at(i).i;
    if((abs(t1) == 3)&&((op_temp==3)/*||(op_temp==1)||(op_temp==3)*/)){
      I1.push_back(I.at(i));
    }
  }
  return I1;
}


double implHard::GetNewGlobalAlphaSh(double energy){
 return 0.9f*alpha;
}

float implHard::S(float fi){
  float temp1 = 1-pow(M2/M1*sin(fi*3.14/180),2);
  if(temp1<0){return 0;}
  return pow(temp1,0.5);
}

float implHard::P(int fi1,int energy){

  float mt = energy*0.00792;
  unsigned int indexE  = 0;
  unsigned int indexFi = 0;
  for(unsigned int i=0;i<listE.size();++i){
    if(mt<=listE.at(i)){indexE=i;break;}
  }
  for(unsigned int i=0;i<listFi.size();++i){
    if(fi1<=listFi.at(i)){indexFi=i;break;}
  }
float d = listT[indexFi].at(indexE);
  return listT[indexFi].at(indexE);
//  float al = listF[ind].at(mt);
//  al=3.14*al;
//  for(unsigned int i=0;i<listFi.size();++i){
//    if(al<=listFi.at(i)){return listT[i].at(mt);}
//  }
//  return 0;
}

float implHard::Pk(float fi1,float fi2,float energy){
 return (P(fi1,energy)-P(fi2,energy))/Pmax;
}


float implHard::Delta(float fi1,float fi2,float energy){
  float mass = 2*M1*M2/(pow(M1+M2,2));
  float sumPk = Pk(fi1,fi1+16,energy) + Pk(fi2,fi2+16,energy);
  float sum_s = (pow(S(fi1),2)+S(fi1)*cos(fi1*3.14/180))*Pk(fi1,fi1+16,energy) + (pow(S(fi2),2)+S(fi2)*cos(fi2*3.14/180))*Pk(fi2,fi2+16,energy); 
  if(energy>0){}
  return mass*sum_s/sumPk;
};

CListI implHard::GetI2(CListI I){
  CListI I1;
  I1.clear();
  
  for(unsigned char i=0;i<I.size();++i){
    signed char op_temp = I.at(i).ptr_automate->op;
    signed char t1 = (signed char)I.at(i).ptr_automate->v-(signed char)I.at(i).i;
	if((abs(t1) == 3)&&((op_temp==-1)||(op_temp==1)/*||(op_temp==3)*/)){
      I1.push_back(I.at(i));
    }
  }
  return I1;
}

 unsigned char implHard::GetDeltai(unsigned char vi,unsigned char i){
return 0;
 }
  
unsigned char implHard::BitEpsilon(unsigned char eps){
if(eps>= Eth){return 1;}
  else{return 0;}
}


double implHard::GetNewGlobalBetaSh(double energy){
  return beta_sh;
}
 
unsigned char implHard::GetNextP(CListI I1, CListI I2,CListI I3){
  unsigned char SumI = I1.size() + I2.size() + I3.size();
  if(SumI>0){
    return p + I1.size() + I2.size() + I3.size() - 1;
  }
  else{
    return p;
  }
}
 
unsigned char implHard::GetNextEpsilon(CListImplHard np,CListI I2,CListI I3){
  float sum1 = 0,sum2= 0,sum3 = 0,sum4 = 0;
  for(unsigned char i=0;i<I2.size();++i){
    sum1 += I2.at(i).ptr_automate->e;
  }
  sum1 *= alpha_sh;
  for(unsigned char i=0;i<I3.size();++i){
    sum2 += I3.at(i).ptr_automate->e;
  }
  sum2 *= beta_sh;
  for(unsigned char i=0;i<np.size();++i){
     if(np.at(i)!=0){
       sum3 += 1;
       sum4 += np.at(i)->epsilon*BitEpsilon(np.at(i)->epsilon);
     }
  }
  sum3 *= gamma*epsilon;
  sum4 *= gamma;
  sum1 += sum2 - sum3 + sum4;
  return epsilon + round(sum1);
}
 
 unsigned char implHard::FuncMod(signed int number, unsigned int module){
if(module<0){return 0;}
	 if(number<0){
       signed char num1 = number+module; 
	   return num1%module;
	 }
	 else{
	   return number%module;
	 }
 }
 
CListI implHard::GetI3(CListI I){
  CListI I1;
  I1.clear();
  for(unsigned char i=0;i<I.size();++i){
    signed char op_temp = I.at(i).ptr_automate->op;
    signed char t2 = (I.at(i).ptr_automate->v + (op_temp/2));
    signed char t1 = abs((FuncMod(t2,6)-I.at(i).i));
    if((abs(t1) == 3)&&((op_temp==-2)||(op_temp==2))){
      I1.push_back(I.at(i));
    }
  }
  return I1;
}
 
 unsigned char implHard::GetNextV(CListI I2,CListI I3,CListI I4){
  ListUnsignedChar ListChar;
   ListChar.clear();
   for(unsigned char i = 0;i<I3.size();++i){
     ListChar.push_back(I3.at(i).ptr_automate->v);
   }
   for(unsigned char i = 0;i<I4.size();++i){
     ListChar.push_back(I4.at(i).ptr_automate->v);
   }
   for(unsigned char i = 0;i<I2.size();++i){
     signed char op_temp = I2.at(i).ptr_automate->op; 
     if(op_temp == 3){
       ListChar.push_back(I2.at(i).ptr_automate->v);	 
	 }
	 else if((op_temp == -1)||(op_temp ==  1)){
	    ListChar.push_back(FuncMod(op_temp+I2.at(i).ptr_automate->v,6));
	 }
   }
   if(ListChar.size() == 0){return 0;}
   else{return ListChar.at(rand()%ListChar.size());}
 }

 
bool implHard::BitDropEnergy(){
 if((op == 0)&&(e>0)){return true;}
  else{return false;}
}
 
unsigned char implHard::GetNextE(CListI I1, CListI I2,CListI I3){
  float sum1 = 0,sum2 = 0,sum3 = 0;
for(unsigned char i=0;i<I1.size();++i){
    sum1 += I1.at(i).ptr_automate->e;
  }  
for(unsigned char i=0;i<I2.size();++i){
    sum2+=I2.at(i).ptr_automate->e;
  }
  for(unsigned char i=0;i<I3.size();++i){
    sum3+=I3.at(i).ptr_automate->e;
  }
  unsigned char last_e = e;
  e = sum1;
  W        = GetNewGlobalW(e);                                                 //тут возвращаем новые значения глобальных переменных
  Omega    = GetNewGlobalOmega(e);
  alpha    = GetNewGlobalAlpha(e);
  beta     = GetNewGlobalBeta(e);  
  float mul_sum1 = (sum1*(1-alpha*0.25));
  e = sum2;
  W        = GetNewGlobalW(e);                                                 //тут возвращаем новые значения глобальных переменных
  Omega    = GetNewGlobalOmega(e);
  alpha    = GetNewGlobalAlpha(e);
  beta     = GetNewGlobalBeta(e);  
  float mul_sum2 = (sum2*(1-beta*0.25));
  e = sum3;
  W        = GetNewGlobalW(e);                                                 //тут возвращаем новые значения глобальных переменных
  Omega    = GetNewGlobalOmega(e);
  alpha    = GetNewGlobalAlpha(e);
  beta     = GetNewGlobalBeta(e);  
  
  float mul_sum3 = (sum3*(1-gamma/**0.25*/));
  e = last_e;
  int e_sum = ((int)mul_sum1+(int)mul_sum2+(int)mul_sum3);

  unsigned char result = FuncMod(e_sum,256);
  if(result == 0){
    if((sum1>0)||(sum2>0)||(sum3>0)){result = 1;}
  }
  return result;
}


CListEpsiloni implHard::GetEpsiloni(CListImplHard np){
  float sum1 = 0,sum2= 0,sum3 = 0;
  CListEpsiloni ListEpsiloni;
  ListEpsiloni.clear();
  for(unsigned char i=0;i<np.size();++i){
    if(np.at(i)!=0){
      float CurP = (rand()%P_MAX)/((float)P_MAX);
      if     (CurP<(1-W-Omega))	    {ListEpsiloni.push_back(0);}
      else if(CurP<(1-W/2-Omega))	{ListEpsiloni.push_back(1);}
      else if(CurP<(1-Omega))		{ListEpsiloni.push_back(-1);}
      else                          {ListEpsiloni.push_back(-np.at(i)->v);}
    }
    else{ListEpsiloni.push_back(0);}
  }
  return ListEpsiloni;
}
   
signed char implHard::GetOp(){
    if(e<=Eth){return 0;}
    else{return GetEps();}
  };




//непонятки тут
signed char implHard::GetEps(){
// double eps = genrand_real1();  
//  float eps = rand()%P_MAX;
//  eps /= P_MAX;
//  return 3;
//    return 2;
    float ver = rand()%P_MAX;
    ver /= P_MAX;
    ver = ver*1.36;
//fprintf(file_save,"m1\n");fflush(file_save);
    for(unsigned int i=0;i<listP.size();++i){
      if(ver<=listP.at(i)){
        ind = i;break;
      }
    }
  float mt = e*0.00793;
  unsigned int indexE  = 0;
  for(unsigned int i=0;i<listE.size();++i){
    if(mt<=listE.at(i)){indexE=i;break;}
  }
//fprintf(file_save,"a = %i; b= %i\n",ind, indexE);fflush(file_save);
  mt = listF[ind].at(indexE)/3.14*180;
 //mt *= 1000.70f;
  if(mt<=5){
	  if(rand()%2){return -1;}
	  else{return 1;}
	}
	else if(mt>=60){
	  if(rand()%2){return 2;}
	  else{return 2;}
	}
	else{return 3;}
}

implHard::implHard(uint param0, uint param1, uint param2, uint param3):CA(param0,param1,param2, param3){
  type = 0;
}
    
CA *implHard::localRule(CA *ptrCA,CListInt  *listNumCA, const CListCA *listCA, unsigned long int step){
  unsigned long int Id_cell = id;
  unsigned long int heigth,width,length;
  unsigned long int _Id;
  heigth = id/(LENGTH*WIDTH);
  _Id = id%(LENGTH*WIDTH);
  width = _Id/LENGTH;
  length = _Id%LENGTH;

  unsigned long int NID;
  int fnum_row = id/LENGTH;                                                    //опредялем позицию частицы
  int fnum_collum = id%LENGTH;
  fprintf(history,"step = %i ,%i\n",step,fnum_collum);
  fflush(history);
  //преобразвание типов
  implHard *inputCA = dynamic_cast<implHard *>(ptrCA);
  CListImplHard listImplHard;
  listImplHard.clear();
  if(inputCA!=0){
	//копирование автомата
    *inputCA = *this;
	//подготовительный этап по формированию указателя на соседей
    int  ssize=listNumCA->size();
    for(unsigned char i=0;i<ssize;++i){
	  if(listNumCA->at(i) == -1){
		listImplHard.push_back(0);  
	  }
	  else{
	    implHard *nCA = dynamic_cast<implHard *>(listCA->at(listNumCA->at(i)));
		listImplHard.push_back(nCA);
	  }
	}
    W        = GetNewGlobalW(e);                                                 //тут возвращаем новые значения глобальных переменных
    Omega    = GetNewGlobalOmega(e);
    alpha    = GetNewGlobalAlpha(e);
    beta     = GetNewGlobalBeta(e);
    if(alpha>1){alpha = 0.9f;}
    if(beta>1){beta =0.9f;}
    alpha_sh = 0.9f*alpha;
    beta_sh  = 0.9f*beta;

  if(((step%STEP_GENERATION_PARTICULAR)==1)&&(step  <MAX_STEP_CREATE_PARTICULAR)&&(fnum_collum==0)){ //это шаг генерации частиц и шаг меньше максимального шага генерация(шаг на котором заканчивается генерация частиц) и соответсвенно край поля
    for(unsigned char i=0;i<MAX_PARTICULAR;++i){                               //проходим по всем частицам и смотрим совпадает ли позиция "виртуальной" частицы с текущей, если совпадает то частица физически появляется
      fprintf(history,"%i ",nl[i]);
	  if(fnum_row == nl[i]){
        inputCA->v = 1;
        inputCA->e=255;
        inputCA->epsilon = 170;
        inputCA->p=0;
        inputCA->op=3;

        v =  1;
        e = 255;
        epsilon = 255;
        p = 200;
        op = 0;
        W        = GetNewGlobalW(e);                                                 //тут возвращаем новые значения глобальных переменных
        Omega    = GetNewGlobalOmega(e);
        alpha    = GetNewGlobalAlpha(e);
        beta     = GetNewGlobalBeta(e);
        alpha_sh = 0.9f*alpha;
        beta_sh  = 0.9f*beta;  
      }
    }
  }	
	fflush(history);
	if((step+1)%2){                                                           //первый полуход
      CListI I1 =  GetI1(listImplHard);                                                   //генерируем необходимые списки
      CListI I2 =  GetI2(I1);
      CListI I3 =  GetI3(I1);
      CListI I4 =  GetI4(I1);
      if(BitDropEnergy()){                                                      //необходимо ли сбросить частицу в ноль?
	    inputCA->e =0;
        inputCA->p = p+1;
	  }	  
	  else{                                                                     //если сброс энергии не нужен то формируем новый импульс и энергию
	    inputCA->p = GetNextP(I2,I3,I4);                                    //формируем импульс
	    inputCA->e = GetNextE(I2,I3,I4);
      } 
	  inputCA->v = GetNextV(I2,I3,I4);                                      //формируем новую скорость
      inputCA->epsilon = GetNextEpsilon(listImplHard,I2,I3);
	}
	else{
	  inputCA->op = GetOp();
	}
  }

  return ptrCA;
}

void implHard::copy(CA *ca){
  implHard *Ca = dynamic_cast<implHard *>(ca);
  if(Ca != 0){
    *this =*Ca;
  }
}




void implHard::geometry(CListInt *ptr){
   bool bit_find = false;
   unsigned long int NID;
   int fnum_row = id/LENGTH;
   int fnum_collum = id%LENGTH;
   bool fbit_even_row = 1&fnum_row;
   if(!fbit_even_row){
 	   NID = GetId(fnum_row-1,fnum_collum,&bit_find);
	   if(bit_find){ptr->push_back(NID);}
	   else{ptr->push_back((unsigned long int) -1);}
	   NID = GetId(fnum_row,fnum_collum+1,&bit_find);
	   if(bit_find){ptr->push_back(NID);}
	   else{ptr->push_back((unsigned long int) -1);}
	   NID =GetId(fnum_row+1,fnum_collum,&bit_find);
	   if(bit_find){ptr->push_back(NID);}
	   else{ptr->push_back((unsigned long int) -1);}
	   NID = GetId(fnum_row+1,fnum_collum-1,&bit_find);
	   if(bit_find){ptr->push_back(NID);}
	   else{ptr->push_back((unsigned long int) -1);}
       	   NID = GetId(fnum_row,fnum_collum-1,&bit_find);
	   if(bit_find){ptr->push_back(NID);}
	   else{ptr->push_back((unsigned long int) -1);}
 	   NID =GetId(fnum_row-1,fnum_collum-1,&bit_find);
	   if(bit_find){ptr->push_back(NID);}
	   else{ptr->push_back((unsigned long int) -1);}
  }
  else{
	   NID = GetId(fnum_row-1,fnum_collum+1,&bit_find);
	   if(bit_find){ptr->push_back(NID);}
	   else{ptr->push_back((unsigned long int) -1);}
	   NID = GetId(fnum_row,fnum_collum+1,&bit_find);
	   if(bit_find){ptr->push_back(NID);}
	   else{ptr->push_back((unsigned long int) -1);}
	   NID = GetId(fnum_row+1,fnum_collum+1,&bit_find);
	   if(bit_find){ptr->push_back(NID);}
	   else{ptr->push_back((unsigned long int) -1);}
	   NID = GetId(fnum_row+1,fnum_collum,&bit_find);
	   if(bit_find){ptr->push_back(NID);}
	   else{ptr->push_back((unsigned long int) -1);}
	   NID = GetId(fnum_row,fnum_collum-1,&bit_find);
	   if(bit_find){ptr->push_back(NID);}
	   else{ptr->push_back((unsigned long int) -1);}
           NID = GetId(fnum_row-1,fnum_collum,&bit_find);
	   if(bit_find){ptr->push_back(NID);}
	   else{ptr->push_back((unsigned long int) -1);}
  }
}


//    void *getParam(uchar *str, uint size){return 0;}
void implHard::init(uint param){
  p = 0;
  e = 0;
  epsilon = 0;
  v = 0;
  op = 0;
  ver = 0;
  ind = 0;
  ugl = 0;   
}
    
//вернусть цевт клетки
CColor implHard::getColor(){
  CColor Color;
  Color.red = 0;
  Color.green = 0;
  Color.blue = 0;
  if(numVisualisation == 0){
  if(e>0){
    Color.red = 0;
    Color.green = 0;
    Color.blue = 255;
  if(v == 0){
    Color.red = 255;
    Color.green = 0;
    Color.blue = 0;
  }
  if(v == 1){
    Color.red = 255;
    Color.green = 0;
    Color.blue = 255;
  }
  if(v == 2){
    Color.red = 255;
    Color.green = 255;
    Color.blue = 0;
  }
  if(v == 3){
    Color.red = 100;
    Color.green = 0;
    Color.blue = 0;
  }
  if(v == 4){
    Color.red = 0;
    Color.green = 0;
    Color.blue = 100;
  }
  if(v == 5){
    Color.red = 100;
    Color.green = 0;
    Color.blue = 100;
  }
  if(v>5){
    Color.red = 254;
    Color.green = 254;
    Color.blue = 254;
  }  
  }
  }
  else  if(numVisualisation == 1){
	  Color.red = 0;
	  Color.green = 100;
	  Color.blue = 0; 
  } 
  
  return Color;
}
