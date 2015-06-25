#include <CA.h>

//метод инициализации клетки
void CA::init(uint,uint) {};
//локальные правила клетки
CA *CA::localRule(CA *ptrCA,CListInt  *listNumCA, const CListCA *listCA, unsigned long int step) {};
//геометрия список соседий
void CA::geometry(CListInt *ptr) {};
//получение параметра, будет использоваться в будущем
//на данный момент не используеься
//void *CA::getParam(uchar *str,uint size){return 0;};
//конструктор класса, задается id
CA::CA(uint param0, uint param1, uint param2, uint param3 ){
  id = param0;
  l = param1;
  w = param2;
  h = param3;
}
void CA::copy(CA *ca){
  *this = *ca;
}
//вернусть цевт клетки
CColor CA::getColor(){
  CColor color;
  color.red = 255;
  color.green = 255;
  color.blue = 255;
  return color;
}
