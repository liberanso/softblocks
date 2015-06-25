#include <vector>
#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <algorithm>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#ifndef DEF_STRUCT_H
#define DEF_STRUCT_H

//���������� ����������� ���������� ����
using namespace std;

//����������
typedef unsigned long int uint;
typedef unsigned char uchar;

//��������� � ������������
struct Coordinate_t
{
    float x;						//������� �� x
    float y;						//������� �� y
    float z;						//������� �� z
};

//������ ���������
typedef vector<Coordinate_t> Coordinates_t;
//������ ������
typedef vector<Coordinates_t> Faces_t;
//������ ����, ����������� �����
typedef vector<Faces_t> Figure_t;

//������ id ���� �����
typedef vector <signed long int> CListInt;
//������ ��������
typedef vector <CListInt> CListVariety;
//������ ���������� ������ ��������� ���������
class CA;
class CThread;

//������ ��������� � �������
typedef vector<CA *> CListCA;
//������ �������, ��� �� � ������ ������ ����� ��, ������� �������� ���������������
//(���� �� ������)
typedef vector<CThread *> CListThread;
typedef vector<float> ClistFloat;

struct CColor
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};
#endif