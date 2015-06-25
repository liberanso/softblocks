#include "CThreadCA2.h"
#include "CA2.h"
#include "settingImplHard.h"
#define MAX_NUM_CENTER 14

unsigned long int nStep;
//генерируем координаты
void CThreadCA2::genSquare(float x, float y,Coordinates_t *ptr){
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
void CThreadCA2::genGrid(){
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
      cur_x = 0;
      cur_x = 10;
      cur_y += 20;
    }
  }
}


void CThreadCA2::init(){
  nStep = 0;
  step =-1;
  long ltime = time(NULL);
		srand(ltime);
	    
  //ячейки lxwxh
  l = LENGTH;
  w = WIDTH;
  h = 1;
  //создаем КА myCA
  ptrCA = new ca2(0,l,w,h);
  uint id = 0;
  //находим общее количество клеток
  numCA = l*w*h;
  //очищаем КА1, и КА2 которые потом будут обмениваться
  listCA1.clear();
  listCA2.clear();
  for(unsigned long int i=0; i<numCA; ++i){
    //создаем два автомата и инициализируем их
    listCA1.push_back(new ca2(id,l,w,h));
    listCA1.at(id)->init(id);
    listCA2.push_back(new ca2(id,l,w,h));
    listCA2.at(id)->init(id);
      ++id;
  }
  //обозначаем выходной автомат
  ptrOutputListCA = &listCA1;
  //генерируем сетку
  genGrid();
}

//добавляем id
void CThreadCA2::addId(CListInt *ptr,signed long int x, signed long int y){
  if((x>=0)&&(x<l)){
    if((y>=0)&&(y<w)){
      ptr->push_back(y*l + x);
    }
  }
}

signed long int CThreadCA2::abs(signed long int x){
  if(x<0){
    return -x;
  }
  else{
    return x;
  }
}

//получить список id
CListInt CThreadCA2::getId(signed long int x, signed long int y,char mode){
  CListInt listId;
  listId.clear();
  if(mode == 0){/*
    addId(&listId,x,y);
    addId(&listId,x-1,y);
    //addId(&listId,x+1,y);
    //addId(&listId,x-1,y+1);
    //addId(&listId,x-1,y-1);
    //addId(&listId,x,y+1);
    //addId(&listId,x,y-1);
	*/
	addId(&listId,x,y);
    addId(&listId,x-1,y);
    //addId(&listId,x+1,y);
    //addId(&listId,x,y+1);
    addId(&listId,x+1,y+1);
    //addId(&listId,x,y-1);
    addId(&listId,x+1,y-1);
  }
  else{
    addId(&listId,x,y);
    //addId(&listId,x-1,y);
    addId(&listId,x+1,y);
    addId(&listId,x,y+1);
    //addId(&listId,x+1,y+1);
    addId(&listId,x,y-1);
    //addId(&listId,x+1,y-1);
  }
  return listId;
}

void CThreadCA2::run(){
  ++nStep;
  CListVariety listVariety;
  CListInt listInt;
  //очищаем множество
  listVariety.clear();
  if(/*(step&1) == 0*/1){
    ptrCurListCA = &listCA1;
    ptrNextListCA = &listCA2;
  }
  else{
    ptrCurListCA = &listCA2;
    ptrNextListCA = &listCA1;
  }
  unsigned long int mode = 0;
  //проходим по всей ширине включая мнимые точки за и после 
  for(int i=-2;(int)i<(int(w+2));i+=2,++mode){
    signed long int x = -2;
	while((int)x<(int)(l+2)){
	  listInt = getId(x,i,(mode+nStep)%2);
	}
  }
  
  /*
  for(int i = -1; (int)i<(int)(w+1); ++i){
    signed long int x;
    if(i<0){
      x = center[i+MAX_NUM_CENTER];
    }
    else{
      x = center[i%MAX_NUM_CENTER];
    }
    while((int)x<(int)(l+1)){
      if(num == 0){
        listInt = getId(x-2,i);
      }
      if(num == 1){
        listInt = getId(x-1,i);
      }
      if(num == 2){
        listInt = getId(x,i);
      }
      if(num == 3){
        listInt = getId(x+1,i);
      }
      if(num == 4){
        listInt = getId(x,i);
      }
      if(num == 5){
        listInt = getId(x-1,i);
      }
      listVariety.push_back(listInt);
      x+=7;
    }
  }
  */
  for(unsigned long int i=0; i<listVariety.size(); ++i){
    listInt = listVariety.at(i);
    for(unsigned long int j=0; j<listInt.size(); ++j){
      unsigned long int n = listInt.at(j);
      (ptrNextListCA->at(n)->copy((ptrCurListCA->at(n)->localRule(ptrCA,&listInt,ptrCurListCA,step))));
    }
  }
  ptrOutputListCA = ptrNextListCA;
}

unsigned char CThreadCA2::getNumModeVisualisation(){
  return 2;
}
