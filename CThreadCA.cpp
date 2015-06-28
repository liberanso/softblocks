#include "CThreadCA.h"
#include "myCA.h"
#include "stdio.h"
#define MAX_NUM_CENTER 14


FILE *file = fopen("result.txt","w"); 
FILE *res = fopen("disp500.txt","a");
FILE *blocks = fopen("blocks.txt","a");
double eps=1e-4; //CO, по достижении которого перезапускаем процесс 
int ckw=1; //переменнаЯ длЯ указаниЯ типа обхода в блоке: 1 - ckw, 0 - anticlockwise

unsigned long int nStep;
int defectOn=10; //0 - без дефекта, 1 - есть случайный дефект
//генерируем координаты
void CThreadCA::genSquare(float x, float y,Coordinates_t *ptr){
  Coordinate_t cord;
  cord.x = x;
  cord.y = y+10.0f;
  cord.z = 0;
  ptr->push_back(cord);
  cord.x = x + 8.66f;
  cord.y = y + 5.0f;
  ptr->push_back(cord);
  cord.x = x + 8.66f;
  cord.y = y - 5.0f;
  ptr->push_back(cord);
  cord.x = x;
  cord.y = y - 10.0f;
  ptr->push_back(cord);
  cord.x = x - 8.66f;
  cord.y = y - 5.0f;
  ptr->push_back(cord);
  cord.x = x - 8.66f;
  cord.y = y + 5.0f;
  ptr->push_back(cord);
  }

//генерируем сетку
void CThreadCA::genGrid(){
  Coordinates_t listCoordinates;
  Faces_t       listFaces;
  listFigure.clear();
  float cur_x = 0;
  float cur_y = 0;
  listCoordinates.clear();
  listFaces.clear();
  for(unsigned long int i=0; i<l; ++i){
    for(unsigned long int j=0; j<w; ++j){
      listCoordinates.clear();
      genSquare(cur_x,cur_y,&listCoordinates);
      listFaces.push_back(listCoordinates);
      listFigure.push_back(listFaces);
      listFaces.clear();
      cur_x += 20;
    }
    if(i%2){
      cur_x = 0;
      cur_y += 20;
    }
    else{
      cur_x = 10;
      cur_y += 20;
    }
  }
}


void CThreadCA::init(){  
  nStep=0;
  long ltime = time(NULL);
		srand(ltime);
	    
  //размерность поля l * w * h
  l = 50;
  w = 50;
  h = 1;
  hasDefect=0; // 0 - no defects anywhere
  complBlocks=0;
  uncomplBlocks=0;
  //создаем КА myCA
  ptrCA = new myCa(0,l,w,h);
  uint id = 0;
  //находим общее количество клеток
  numCA = l*w*h;
  //очищаем КА1, и КА2 которые потом будут обмениваться
  listCA1.clear();
  listCA2.clear();
//  int def_num = rand()%(w*l*h);
  int def_pos[]={2233,2205,2356,70,103,2452,1098,1402,495,1596};
 /* for(int i=0;i<defectOn;i++) { //для рандомной генерации нужного числа дефектов
	def_pos[i]=rand()%(w*l*h);
while (def_pos[i]>=(w-10)*(l/2) && def_pos[i]<=(w+10)*l/2 && def_pos[i]%w>=(l/2-10/2) && def_pos[i]%w<(l/2+10/2)) {
	  def_pos[i]=rand()%(w*l*h);
  }
  }
  */
  ////////////////////////////////////////////
/*  while (def_num>=(w-10)*(l/2) && def_num<=(w+10)*l/2 && def_num%w>=(l/2-10/2) && def_num%w<(l/2+10/2)) {
	  def_num=rand()%(w*l*h);
  }
на удаление идет*/ ////////////////////////// 
  for(unsigned long int i=0; i<numCA; ++i){
    //создаем два автомата и инициализируем их
    listCA1.push_back(new myCa(id,l,w,h));
    listCA1.at(id)->init(id,def_pos,defectOn);
    listCA2.push_back(new myCa(id,l,w,h));
    listCA2.at(id)->init(id, def_pos,defectOn);
      ++id;
  }
  //обозначаем выходной автомат
  ptrOutputListCA = &listCA1;
  //генерируем сетку
  genGrid();
}

//добавляем id
bool CThreadCA::addId(CListInt *ptr,signed long int x, signed long int y){
  if((x>=0)&&(x<l)){
    if((y>=0)&&(y<w)){
      ptr->push_back(y*l + x); 
    return 1;
	} else return 0;
  } else return 0;
    }

signed long int CThreadCA::abs(signed long int x){
  if(x<0){
    return -x;
  }
  else{
    return x;
  }
}

//получить список id
CListInt CThreadCA::getId(signed long int x, signed long int y){
  CListInt listId;
  listId.clear();

	bool isComplete=1; //block is complete сбрасываем в 0, только когда хоть одна Ячейка не добавитсЯ в блок 
	int allCells=4; 

  if(ckw==1) { //если обход внутри блоков по час стр
  if( ((y)/2+nStep)%2 == 0){
	
	bool isAdd=addId(&listId,x,y); 
	if(isAdd==0) {
	isComplete=0;
	allCells--;
}	
    isAdd=addId(&listId,x-1,y);
    if(isAdd==0) {
	isComplete=0;
	allCells--;
}	
    isAdd=addId(&listId,x,y+1);
    if(isAdd==0) {
	isComplete=0;
	allCells--;
}	
    isAdd=addId(&listId,x,y-1);
if(isAdd==0) {
	isComplete=0;
	allCells--;
}	
    if(isComplete==1) { //если весь блок записался, 
	  complBlocks++;
  } else {
	  if(allCells!=0){ //если блок хотЯ бы одной Ячейкой лежит на поле, но тем не менее не полностью
	  uncomplBlocks++; 
  }
  }
   }
  else{
    bool isAdd=addId(&listId,x,y); 
	if(isAdd==0) {
	isComplete=0;
	allCells--;
}	
    isAdd=addId(&listId,x+1,y);
	if(isAdd==0) {
	isComplete=0;
	allCells--;
}	
    isAdd=addId(&listId,x-1,y-1);
	if(isAdd==0) {
	isComplete=0;
	allCells--;
}	
    isAdd=addId(&listId,x-1,y+1);
	if(isAdd==0) {
	isComplete=0;
	allCells--;
}	
	if(isComplete==1) { //если весь блок записался, 
	  complBlocks++;
  } else {
	  if(allCells!=0){ //если блок хотЯ бы одной Ячейкой лежит на поле, но тем не менее не полностью
	  uncomplBlocks++; 
  }
  }
	}
	} else if(ckw==0) { //если обход внутри блоков против час стр 
		if( ((y)/2+nStep)%2 == 0){
	
	bool isAdd=addId(&listId,x,y);	
    addId(&listId,x-1,y);
    addId(&listId,x,y-1);
	addId(&listId,x,y+1);
        
  }
  else{
    addId(&listId,x,y);
    addId(&listId,x+1,y);
    addId(&listId,x-1,y+1);
	addId(&listId,x-1,y-1);
	    
	}
	}
  return listId; //return list of neighbors coordinates
}

bool CThreadCA::run(){

  ++nStep;
  CListVariety listVariety;
  CListInt listInt;
  //очищаем множество
  listVariety.clear();
  int centergo=0; //переменная для указания вектора смещения центров всех блоков на текущем шаге: 0 без смещения
    ptrCurListCA = &listCA2;
    ptrNextListCA = &listCA1;
/*	
if(nStep%2==0) { //even step четный шаг
	 // centergo=rand()%6; //смещение центра в (0,1,2,3,4,5,6)
	  //or
	  //centergo=1; //смещение центров в первом направлении
	  //or
	 // centergo=2; //во втором
	  //or
	 // centergo=3; //в третьем
	  //or
	  // centergo=4; //в четвертом
	  //or
	  // centergo=5; //в п\том
	  //or
	  // centergo=6; //в шестом
	  //or
	// centergo=rand()%6+1; //смещение центра в (1,2,3,4,5,6)
  } else if(nStep%2!=0) { //нечетный шаг odd step  
	  centergo=0; //без смещения центра 
  } 
  */
  if(nStep%3==1) { 
	  centergo=1;
  } else if(nStep%3==2){
	  centergo=3;
  } else if(nStep%3==0) {
	  centergo=5;
  }
//  centergo=rand()%6+1; //смещение центра в (1,2,3,4,5,6)
 /*****************************************************************************************************************************************
Изначально каждая из ячеек цикла (+2 по иксу, +2 по игреку на каждом шаге) - центр у-блока. 
В зависимости от значениџ centergo, мы смещаем центр каждого блока в одном из трех направлений, которые повторяются циклически.
На каждом шаге блоки чередуются, т.о., они накладываются на соседних шагах. 

ckw - переменная, показывающая тип обхода блока.
 ********************************************************************************************************************************************/
  	
  //проходим по всей ширине, включая мнимые точки за и после 
  for(int y=-2;(int)y<(int(w+2));y+=2){ // i = -2:2:w+2 
    
	for(int x=-2;(int)x<(int)(l+2);x+=2){ //while x < l+2 
if(centergo==0) {	 
	 listInt = getId(x,y); //list of neighbors coordinates, add only those that belong to field 
	} else if(centergo==1) {
	listInt=getId(x-1,y);
	} else if(centergo==2) { //*FIXME*
		listInt=getId(x+1,y);
	} else if(centergo==3) {
		listInt=getId(x+1,y+1);
	} else if(centergo==4) { //*FIXME*
		listInt=getId(x-1,y-1);
	} else if(centergo==5) {
		listInt=getId(x+1,y-1);
	} else if(centergo==6) { //*FIXME* 
		listInt=getId(x-1,y+1);
	}
	  listVariety.push_back(listInt); //push this list to listVariety 
	  listInt.clear(); 
	}
}  
 
  for(unsigned long int i=0; i<listVariety.size(); ++i){ //for all cells 
    listInt = listVariety.at(i); //take the neighbors list for current cell 

    for(unsigned long int j=0; j<listInt.size(); ++j){ //go around all its neighbors
      unsigned long int n = listInt.at(j); //n is neighbor id 
      	  (ptrNextListCA->at(n)->copy((ptrCurListCA->at(n)->localRule(ptrCA,&listInt,ptrCurListCA,nStep)))); //the cell at next step
	  //is the current cell under localRule (pointer to CA, & to neighbors list for current original cell, ptr to cur list of CAs, step )
	  
	}
	/***************************************************
	*поворот блоков
	*rotationOn==1 - по часовой
	*rotationOn==0 - против часовой стрелки
	***************************************************/
	int rotationOn=rand()%1; //rotationOn is 0 or 1
	//int rotationOn=0; // 1 - по часовой - ckw clockwise //0 - против часовой - anticlockwise
	if(rotationOn==1) {
		// по часовой

                double prev_state=0.0, next_state=0.0;
				unsigned long int num=0;
                for(unsigned long int j=0; j<listInt.size(); ++j){ //go around all original cell neighbors
				num = listInt.at(j); //num is neighbor id 
				myCa* nCA_0 = dynamic_cast<myCa *>(ptrCurListCA->at(num));
                if(nCA_0!=0 && nCA_0->state!=7)
                {
                   prev_state=nCA_0->state;
				   j++; 
				   if(j<listInt.size()) {
					   num=listInt.at(j); //n is next neighbour id 
				       myCa *nCA_1 = dynamic_cast<myCa *>(ptrCurListCA->at(num));
                       if(nCA_1!=0 && nCA_1->state!=7)
					   {
                        next_state = nCA_1->state;
                        nCA_1->state=prev_state;
                        prev_state=next_state;
                       } else if (nCA_1->state==7 || nCA_1==0) {
						    break;
					        }
                nCA_0->state=prev_state;
				}
				
	         }
  }
  }	else if(rotationOn==0)
  {
	            double prev_state=0.0, next_state=0.0;
				long int num=listInt.size()-1;
				if(num>0) {
					for(long int j=listInt.size()-1;j>=0; --j){ //go around all original cell neighbors
				num = listInt.at(j); //num is neighbor id 
				myCa* nCA_0 = dynamic_cast<myCa *>(ptrCurListCA->at(num));
               if(nCA_0!=0 && nCA_0->state!=7)
                {
                   prev_state=nCA_0->state;
				   j--; 
				   if(j>=0) {
					   num=listInt.at(j); //n is next neighbor id 
				       myCa *nCA_1 = dynamic_cast<myCa *>(ptrCurListCA->at(num));
                       if(nCA_1!=0 && nCA_1->state!=7)
					   {
                        next_state = nCA_1->state;
                        nCA_1->state=prev_state;
                        prev_state=next_state;
                       } else if (nCA_1->state==7 || nCA_1==0) {
						    break;
					        }
                nCA_0->state=prev_state;
				}
				
	         } 
			 
  }
  }
  }
 
 }
		//считаем дисперсию и СО
		double sum = 0; 
		double dispertion = 0.0;
	
		double state_on_ca=(double)600/(l*w*h-defectOn); //6*10*10-1  l*w*h minus quantity of defects 
					
		for(unsigned int i=0;i<ptrCurListCA->size();++i){ //go around all current CAs 
			
			if((((myCa *)(ptrCurListCA->at(i)))->state)!=7) { //if state of current cell is not 7
		  sum+=((myCa *)(ptrCurListCA->at(i)))->state; //increase the sum by cell's state 
		  dispertion+=pow((double)((myCa *)(ptrCurListCA->at(i)))->state-state_on_ca,2); // dispertion increment is square of (state-0.24) 0.24 - average state 600/2500
		  //fprintf(file,"%f ",((myCa *)(ptrCurListCA->at(i)))->state);		  
		}	
		}
		
		dispertion=pow(dispertion/(l*w*h-defectOn),0.5);	//dispertion is square root of disp/2500
		fprintf(file,"%e\n",dispertion);	//на каждом шаге записываем дисперсию		
		fflush(file);		
				
		if(dispertion<eps)
		{			
			fprintf(res,"%d\n",nStep);			
			fflush(res);
			return false;
		
		}	
	     ++step;
		fprintf(blocks,"complete blocks: %d, uncomplete blocks: %d\n",complBlocks,uncomplBlocks);
		fflush(blocks);
		complBlocks=0;
		uncomplBlocks=0;
  //ptrOutputListCA = ptrNextListCA;
  return true;
}

unsigned char CThreadCA::getNumModeVisualisation(){
  return 2;
}