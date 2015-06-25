#include "CThreadCA.h"
#include "myCA.h"
#include "stdio.h"
#define MAX_NUM_CENTER 14


FILE *file = fopen("result.txt","w"); 
FILE *res = fopen("disp500.txt","a");
FILE *vicin = fopen("vicinity.txt", "a");
//при наличии 
// result1 - //записать значение дисперсии и соотв-го ей шага при отсутствии дефектов 
//result2 - //записать значение дисперсии и соотв-го ей шага при одном дефекте в центре
//result3 - при одном дефекте на границе

unsigned long int nStep;

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
	    
  //ячейки lxwxh
  l = 50;
  w = 50;
  h = 1;
  hasDefect=0; //no defects anywhere
  
  //создаем КА myCA
  ptrCA = new myCa(0,l,w,h); //перваЯ Ячейка ?
  uint id = 0;
  //находим общее количество клеток
  numCA = l*w*h;
  //очищаем КА1, и КА2 которые потом будут обмениваться
  listCA1.clear();
  listCA2.clear();
  int def_num = rand()%2500;
  while (def_num>=(w-10)*(l/2) && def_num<=(w+10)*l/2 && def_num%w>=(l/2-10/2) && def_num%w<(l/2+10/2)) {
	  def_num=rand()%2500;
  }
  
  for(unsigned long int i=0; i<numCA; ++i){
    //создаем два автомата и инициализируем их
    listCA1.push_back(new myCa(id,l,w,h));
    listCA1.at(id)->init(id,def_num);
    listCA2.push_back(new myCa(id,l,w,h));
    listCA2.at(id)->init(id, def_num);
      ++id;
  }
  //обозначаем выходной автомат
  ptrOutputListCA = &listCA1;
  //генерируем сетку
  genGrid();
}

//добавляем id
void CThreadCA::addId(CListInt *ptr,signed long int x, signed long int y){
  if((x>=0)&&(x<l)){
    if((y>=0)&&(y<w)){
      ptr->push_back(y*l + x); 
    }
  }
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
  
  int ckw =1; //1 - ckw, 0 - anticlockwise / нужно ее менЯть отдельно ещЮ и в run() здесь же
  if(ckw==1) {
  if( ((y)/2+nStep)%2 == 0){ //ckw in block //работает длЯ схем нечетнаЯ строка + нечетный шаг или 
	
	addId(&listId,x,y);	  //add 0 cell
	addId(&listId,x-1,y); //add 1 cell 
	addId(&listId,x,y+1); // 2 cell 
	addId(&listId,x,y-1); // 3 cell 
  }
  else{
 addId(&listId,x,y);        //add 0 cell
  addId(&listId,x+1,y);     //add 1 cell
  addId(&listId,x-1,y-1);   // 2 cell 
  addId(&listId,x-1,y+1);   // 3 cell 
    }
  } else if(ckw==0) { //anticlockwise in block 
	  if(((y)/2+nStep)%2 == 0) {
	addId(&listId,x,y);	//add 0 cell
	addId(&listId,x-1,y); //add 1 cell 
	addId(&listId,x,y-1); // 2 cell 
	addId(&listId,x,y+1); // 3 cell
    } else {
addId(&listId,x,y);         //add 0 cell
  addId(&listId,x+1,y);     //add 1 cell
 addId(&listId,x-1,y+1);    // 2 cell 
  addId(&listId,x-1,y-1);   // 3 cell 
    }
  }
  return listId; //return list of neighbors coordinates
}

void CThreadCA::run(){

  ++nStep;
  int centergo=0; //переменнаЯ длЯ указаниЯ вектора смещениЯ центров всех блоков на текущем шаге: 0 без смещениЯ
  
  if(nStep%2==0) { //even step четный шаг
	  centergo=rand()%3; //смещение центра в (0,1,2,3)
	  //or
	  //centergo=1; //смещение центров в первые Ячейки
	  //or
	 // centergo=2; //во вторые
	  //or
	 // centergo=3; //в третьи
	  //or
	// centergo=rand()%3+1; //смещение центра в (1,2,3)
  } else if(nStep%2!=0) { //нечетный шаг odd step  
	  centergo=0; //без смещениЯ центра 
  }
  
 /*****************************************************************************************************************************************
€значально каждаЯ из Ячеек цикла (+2 по иксу, +2 по игреку на каждом шаге) - центр у-блока. 
‚ зависимости от значениЯ centergo, мы смещаем центр каждого блока в одну из периферийных Ячеек изначального блока.
€ли не смещаем, оставлЯЯ центр центром, когда она равна 0 (на нечетном шаге всегда так).
centergo  менЯетсЯ каждый шаг, в зависимости от его четности.
Ќа каждом шаге блоки чередуютсЯ и они накладываютсЯ на соседних шагах. 

ckw - переменнаЯ, показывающаЯ тип обхода блока. ‚ зависимости от направлениЯ обхода и четности/нечетности шага и строки мы и центр смещаем. 
 ********************************************************************************************************************************************/
  
  int ckw=1; //переменнаЯ длЯ указаниЯ типа обхода в блоке: 1 - ckw, 0 - anticlockwise / нужно ее менЯть отдельно ещЮ и в getId() в этом же файле
  CListVariety listVariety;
  CListInt listInt;
  //очищаем множество - список Ячеек с их окрестностЯми 
  listVariety.clear();
  if(/*(nStep&1) == 0*/0){ //никогда не выполнЯетсЯ
    ptrCurListCA = &listCA1;
    ptrNextListCA = &listCA2;
  }
  else
  {
    ptrCurListCA = &listCA2;
    ptrNextListCA = &listCA1;
  }
  //проходим по всей ширине и длине

  for(int y=-2;(int)y<(int(w+2));y+=2){ // y = -2:2:w+2 
    signed long int x = -2;
	
	while((int)x<(int)(l+2)){ //x = -2:2:l+2 
	if(ckw==1) {
	if(centergo==0){
	
	listInt = getId(x,y); //we add center cell as center
	  fprintf(vicin,"%d, %d \n",x,y);
	  fflush(vicin);
	
	} else if(centergo==1) {
		if(((y)/2+nStep)%2 == 0) {
			listInt = getId(x-1,y); //we add 1 cell as center
			
	 } else{
			listInt = getId(x+1,y); //we add 1 cell as center
			
	  }
	}else if(centergo==3) {
		if(((y)/2+nStep)%2 == 0) {
			listInt = getId(x,y-1); //we add 3 cell as center
			
			} else {
			listInt = getId(x-1,y+1); //we add 3 cell as center
			
	  	}
	} else if (centergo==2) {
		if(((y)/2+nStep)%2 == 0) {
			listInt = getId(x,y+1); //we add 2 cell as center
			
	 	} else {
			listInt = getId(x-1,y-1); //we add 2 cell as center
	 		
			
			}
		}
	} else if(ckw==0) { //anticlockwise
		if(centergo==0){
			
	  listInt = getId(x,y); //we add center cell as center
	  fprintf(vicin,"%d, %d \n",x,y);
	  fflush(vicin);

	} else if(centergo==1) {
		if(((y)/2+nStep)%2 == 0) {
			listInt = getId(x-1,y); //we add 1 cell as center
			
	 } else{
			listInt = getId(x+1,y); //we add 1 cell as center
			
	  }
	}else if(centergo==3) {
		if(((y)/2+nStep)%2 == 0) {
			listInt = getId(x,y+1); //we add cell 3 as center
			
			} else {
		listInt = getId(x-1,y-1);	//we add cell 3 as center
	
	  	}
	} else if (centergo==2) {
		if(((y)/2+nStep)%2 == 0) {
			listInt = getId(x,y-1); //we add cell 2 as center
			
	 	} else {
			listInt = getId(x-1,y+1);	// we add cell 2 as center
			
	 		}
		}
	}
	listVariety.push_back(listInt); //push this list to listVariety 
	  listInt.clear(); 
	  x+=2;
	}
  }  
 fprintf(vicin,"%d\n",listVariety.size());
 fflush(vicin);
  for(unsigned long int i=0; i<listVariety.size(); ++i){ //for all center cells 
    listInt = listVariety.at(i); //take the neighbors list for current cell 

    for(unsigned long int j=0; j<listInt.size(); ++j){ //go around all its neighbors
      unsigned long int n = listInt.at(j); //n is neighbor id 
      	  (ptrNextListCA->at(n)->copy((ptrCurListCA->at(n)->localRule(ptrCA,&listInt,ptrCurListCA,nStep)))); //the cell at next step
	  //is the current cell under localRule (pointer to CA, & to neighbors list for current original cell, ptr to cur list of CAs, step )
	  
	}
	int rotationOn=rand()%1; //rotationOn is 0 or 1  - поворот блоков по часовой или против 
	//int rotationOn=1; // 1 - по часовой - ckw clockwise //0 - против часовой - anticlockwise // -1 - без поворота
	if(rotationOn==1) { 
		// по часовой

                double prev_state=0.0, next_state=0.0;
				unsigned long int num=0;
				if(listInt.size()==4) {
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
		double sum = 0; 
		double dispertion = 0.0;
		//?
		double state_on_ca=(double)600/2499; //6*10*10  50*50=2500 minus 1 defect 
					
		for(unsigned int i=0;i<ptrCurListCA->size();++i){ //go around all current CAs 
			
			if((((myCa *)(ptrCurListCA->at(i)))->state)!=7) { //if state of current cell is not 7
		  sum+=((myCa *)(ptrCurListCA->at(i)))->state; //increase the sum by cell's state 
		  dispertion+=pow((double)((myCa *)(ptrCurListCA->at(i)))->state-0.240096,2); // dispertion increment is square of (state-0.24) 0.24 - average state 600/2500
		  //fprintf(file,"%f ",((myCa *)(ptrCurListCA->at(i)))->state);		  
		}	
		}
		
		dispertion=pow(dispertion/2499,0.5);	//dispertion is square root of disp/2500
		fprintf(file,"%e\n",dispertion);	//на каждом шаге записываем дисперсию		
		fflush(file);			
		
		
		
		if(dispertion<1e-6)
		{					
			fprintf(res,"%d\n",nStep);			
			fflush(res);
		//	fclose(res);
		}	
	        
        ++step;
		
  //ptrOutputListCA = ptrNextListCA;
}

unsigned char CThreadCA::getNumModeVisualisation(){
  return 2;
}