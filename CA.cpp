#include <CA.h>

//����� ������������� ������
void CA::init(uint,uint) {};
//��������� ������� ������
CA *CA::localRule(CA *ptrCA,CListInt  *listNumCA, const CListCA *listCA, unsigned long int step) {};
//��������� ������ �������
void CA::geometry(CListInt *ptr) {};
//��������� ���������, ����� �������������� � �������
//�� ������ ������ �� ������������
//void *CA::getParam(uchar *str,uint size){return 0;};
//����������� ������, �������� id
CA::CA(uint param0, uint param1, uint param2, uint param3 ){
  id = param0;
  l = param1;
  w = param2;
  h = param3;
}
void CA::copy(CA *ca){
  *this = *ca;
}
//�������� ���� ������
CColor CA::getColor(){
  CColor color;
  color.red = 255;
  color.green = 255;
  color.blue = 255;
  return color;
}
