#include <myCA.h>
#include "stdio.h"

//просто пример класса

int defOn=4; //0 - без дефекта, 1 - есть случайный дефект  
myCa::myCa(uint param0, uint param1, uint param2, uint param3):CA(param0,param1,param2, param3)
{
    type = 0;
}

CA *myCa::localRule(CA *ptrCA, CListInt  *listNumCA, const CListCA *listCA, unsigned long int step)
{

    myCa *inputCA = dynamic_cast<myCa *>(ptrCA); //check if we can use our cell
    if(inputCA!=0) //если не нулевой указатель
    {
		 
        *inputCA = *this; //присваиваем inputCA значение, находящееся по адресу this

        int  ssize=listNumCA->size(); //length of neighbors list for current cell
        int *array = new int[ssize]; //new array with length of neighbors list for current cell

        if(ssize==4 || if7!=0)//if the original central cell has 4 neighbors (with itself ?) or if7 non zero 
        {
            
			for(int i = 0; i < ssize; i++) //for i < length of neighbors list for current cell
            {
                array[i] = i; //array is 0:1: (length of neighbors list for current original cell)
            }
            //random_shuffle ( array,  array+ssize);
            int color = rand()%4+1;  //color = rand (1,2,3,4) wiil be if7 for all neighbors of current cell if it isn't state7 and they aren't state7 too 
            for(int j=0; j<ssize; ++j) //for j < length of neighbors list for current original cell
            {
				if(state!=7) { //if central cell is not defect 
                double st = state; //take the state of this->state ie. of central cell 
                int b = 3; //? 
                int i=array[j]; //what we need i for? 

                myCa *nCA = dynamic_cast<myCa *>(listCA->at(listNumCA->at(j))); //nCa = one of neighbor cells for  *this cell (for one of neighbors cells of original cell)
                if((nCA!=0)&&(nCA->state!=7)) //if neighbor cell is not defect 
                {
                    double a = st-nCA->state; //central state - neighbor state
                    state=state - a/b; //cenral state = central state - (central state - neighbor state)/3 
                    nCA->state=nCA->state+a/b; //neighbor state = neighbor state + (central state - neighbor state)/3 
                    nCA->if7=color; //neighbor if7 = rand (1,2,3,4)
                }else if(nCA->state==7) { //if neighbor state is 7 we should not do anything for this vicinity
					break;
				}
				
				} else if (state==7){ //if central cell has 7 state
					break; //should not change any cells from this cell's neighbors 
				}
            }
			
		}
        delete[] array;

    }
	
    return ptrCA;
}

void myCa::copy(CA *ca)
{
    myCa *Ca = dynamic_cast<myCa *>(ca);
    if(Ca != 0)
    {
        *this =*Ca;
    }
}

uchar myCa::check(int _x, int _y)
{
    if(id == 1) {}
    if(id == (_x*l+_y))
    {
        return 1;
    }
    else if(id == (_x*l+_y-1))
    {
        return 1;
    }
    else if(id == (_x*l+_y+1))
    {
        return 1;
    }
    else if(id == ((_x+1)*(l)) + _y)
    {
        return 1;
    }
    else if(id == ((_x+1)*l+_y+1))
    {
        return 1;
    }
    else if(id == ((_x-1)*(l)) + _y)
    {
        return 1;
    }
    else if(id == ((_x-1)*l+_y+1))
    {
        return 1;
    }
    return 0;
}

void myCa::add(CListInt *ptr,int _x, int _y)
{

    ptr->push_back(_y*w+_x);
    if(_y+1<w)
    {
        ptr->push_back(((_y+1)*w+_x));
        if(_x+1>0)
        {
            ptr->push_back(((_y+1)*w+_x+1));
        }
    }
    if(_y-1>0)
    {
        ptr->push_back(((_y-1)*w+_x));
        if(_x+1>0)
        {
            ptr->push_back(((_y-1)*w+_x+1));
        }
    }
    if(_x-1>0)
    {
        ptr->push_back(((_y)*w+_x-1));
    }
    if(_x+1>0)
    {
        ptr->push_back(((_y)*w+_x+1));
    }
}

void myCa::geometry(CListInt *ptr)
{
    //коэфициенты найдены практическим способом
    int increment = 14;
    int decrement = 3;
    //начальная точка 0, 0
    int valueBegin = 0;
    int _x = 0;
    int _y = 0;
    int beginX = 0;
    //ptr->push_back(1);
    //fprintf(file, "__ID = %i\n",id);
    for(int i=-3; i<((int)w+3); ++i)
    {
        //ищем центры, 0, 0 начальный центр
        _x = beginX;
        while(_x<((int)w))
        {
            if(check(i,_x))
            {
                printf("end\n");
                //add(ptr,i,_x);
                return;
            }
            else
            {
                _x += increment;
            }
        }

        beginX -= 3;
    }
}


//    void *getParam(uchar *str, uint size){return 0;}
void myCa::init(uint param,uint def_num) //передается параметром айди клетки 
{
    int w_q=10;
    int l_q=10;
    int init_state=6;
	int defect=7; //changed
	//int def_num=439; //changed
	
	
    //w=50
	//l=50
	//h=1 
	
    state=0;
    if7=0;

	
     if(param>=(w-w_q)*(l/2) && param<=(w+w_q)*l/2 && param%w>=(l/2-l_q/2) && param%w<(l/2+l_q/2))
    {
         state=init_state; //impl
    }	
	//if((param==527)||(param==730)||(param==990)||(param==1530)||(param==1620)||(param==2315)) { //for 6 defects 
	//if((param==527)||(param==1530)||(param==1620)||(param==2315)) { //for 4 defects 
	//1620 center
	// 2315 periphery 
	//if((param == 730)) { //for 2 defects 
	//	state=defect;
	//}
	//if(param==2315) { //for 1 defect center 
//		state=defect;
//	}
if(defOn!=0) {
if(param==def_num || param==2340 || param==1254 || param == 1398) {// || param == 2407) { //добавление рандомного дефекта 
	state=defect;
}
}
}

//вернуть цвeт клетки
CColor myCa::getColor()
{
    CColor color;
    color.red = 0;
    color.green = 0;
    color.blue = 0;
    

    if(state <= 0) {}
    else if(state   <=	0.5	)
    {
        color.red = 49;
        color.green =54;
        color.blue =	149	;
    }
    else if(state   <=	1	)
    {
        color.red = 69;
        color.green =117;
        color.blue =	180	;
    }
    else if(state   <=	1.5	)
    {
        color.red = 116;
        color.green =173;
        color.blue =	209	;
    }
    else if(state   <=	2	)
    {
        color.red = 171;
        color.green =217;
        color.blue =	233	;
    }
    else if(state   <=	2.5	)
    {
        color.red = 224;
        color.green =243;
        color.blue =	248	;
    }
    else if(state   <=	3	)
    {
        color.red = 255;
        color.green =255;
        color.blue =	191	;
    }
    else if(state   <=	3.5	)
    {
        color.red = 254;
        color.green =224;
        color.blue =	144	;
    }
    else if(state   <=	4	)
    {
        color.red = 253;
        color.green =174;
        color.blue =	97	;
    }
    else if(state   <=	4.5	)
    {
        color.red = 244;
        color.green =109;
        color.blue =	67	;
    }
    else if(state   <=	5	)
    {
        color.red = 215;
        color.green =48;
        color.blue =	39	;
    }
    else if(state   <=	6	)
    {
        color.red = 165;
        color.green =0;
        color.blue =	38	;
    }
	else if(state==7) {
		color.red=124;
		color.green=252;
		color.blue=00;
	}

    return color;
}
