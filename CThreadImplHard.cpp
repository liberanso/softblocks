#include <CThreadImplHard.h>
#include <stdio.h>

unsigned char numC;
ClistFloat listF[max_col_excel];
ClistFloat listP,listE,listFi,listE2,listT[LENGTH_ROW_T2];
//массив со случайным положением частиц, служит, чтобы потом сгенерировать появляющиеся частицы
unsigned char nl[MAX_PARTICULAR];                       
unsigned int p_collum[LENGTH];
unsigned int p_row[WIDTH];
FILE *f;
FILE *history;
CListCA *listHardImpl;

void readStr(char *a,FILE *file){
  numC = 0;
  unsigned char sim;
  unsigned char begin = 0;
  while(begin == 0){
    sim = fgetc(f);
	if(sim == '0'||sim == '1'||sim == '2'||sim == '3'||sim == '4'||sim == '5'||sim == '6'||sim == '7'||sim == '7'||sim == '8'||sim == '9'||sim == ','||
      sim == '-'||sim == '+'||sim == 'e'||sim == 'E'){begin = 1;}  
  }
  while(begin == 1){
    a[numC] = sim;
	++numC;
    sim = fgetc(f);
	if(sim == '0'||sim == '1'||sim == '2'||sim == '3'||sim == '4'||sim == '5'||sim == '6'||sim == '7'||sim == '7'||sim == '8'||sim == '9'||sim == ','||
      sim == '-'||sim == '+'||sim == 'e'||sim == 'E'){begin = 1;}  
	else{begin = 0;}
  }  
  a[numC] = '\0';
//  static unsigned char k =0;
//  ++k;
//  if(k<10){
//    printf("ind = %i   %s\n",numC,a);
//  }
}

void loadTable()
{
  setlocale(LC_ALL,".ACP");
  f = fopen("a2.csv","r");
  unsigned int param;
  unsigned int index;
  listP.clear();
  listE.clear();
  char str[20];
  for(unsigned int i=0;i<max_row_excel;++i){
    listF[i].clear();
  }
  index=0;
  for(unsigned int i=0;i<LENGTH_ROW_T2;++i){
    for(unsigned int j=0;j<LENGTH_STR_T2;++j){
	  readStr(str,f);
	  if(index == 0){
	    if(j!=0){listE2.push_back(strtod(str,0));}
	  }
	  else if(j==0){listFi.push_back(strtod(str,0));}
	  else{listT[index-1].push_back(strtod(str,0));}
	}
	++index;
  }
  fclose(f);
  f = fopen("a.csv","r");
  index=0;
  for(unsigned int i=0;i<max_col_excel;++i){
    for(unsigned int j=0;j<max_row_excel;++j){
	readStr(str,f);
	if(index == 0){
	    if(j!=0){listE.push_back(strtod(str,0));}
	  }
	  else if(j==0){listP.push_back(strtod(str,0));}
	  else{listF[index-1].push_back(strtod(str,0));}
	}
	++index;
  }
}

//генерируем координаты
void CThreadImplHard::genSquare(float x, float y,Coordinates_t *ptr){
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
void CThreadImplHard::genGrid(){
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


void CThreadImplHard::init(){
  long ltime = time(NULL);
  srand(ltime);
  history = fopen("history.txt","w");
  loadTable();  
  //ячейки lxwxh
  l =  WIDTH;
  w = LENGTH;
  h = 1;
  step = 0;
  //создаем КА implHard
  ptrCA = new implHard(0,l,w,h);
  uint id = 0;
  //находим общее количество клеток
  numCA = l*w*h;
  //очищаем КА1, и КА2 которые потом будут обмениваться
  listCA1.clear();
  listCA2.clear();
  for(unsigned long int i=0; i<numCA; ++i){
    //создаем два автомата и инициализируем их
    listCA1.push_back(new implHard(id,l,w,h));
    listCA1.at(id)->init(id);
    listCA2.push_back(new implHard(id,l,w,h));
    listCA2.at(id)->init(id);
      ++id;
  }
  //обозначаем выходной автомат
  ptrOutputListCA = &listCA1;
  //генерируем сетку
  genGrid();
}

void CThreadImplHard::run(){
 for(unsigned char i=0;i<MAX_PARTICULAR;++i){
   nl[i] = rand()%(WIDTH/3)+WIDTH/3;
 }

  CListVariety listVariety;
  CListInt listInt;
  //очищаем множество
  listVariety.clear();
  if((step&1) == 0){
    ptrCurListCA = &listCA1;
    ptrNextListCA = &listCA2;
  }
  else{
    ptrCurListCA = &listCA2;
    ptrNextListCA = &listCA1;
  }
  for(unsigned long int i=0; i<ptrCurListCA->size(); ++i){
    listInt.clear();
	ptrCurListCA->at(i)->geometry(&listInt);
    (ptrNextListCA->at(i)->copy((ptrCurListCA->at(i)->localRule(ptrCA,&listInt,ptrCurListCA,step))));
  }
  listHardImpl = ptrNextListCA;
  ++step;
}

unsigned char CThreadImplHard::getNumModeVisualisation(){
  return 2;
}