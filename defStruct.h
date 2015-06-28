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

//используем стандартную библиотеку имен
using namespace std;

//Предефайны
typedef unsigned long int uint;
typedef unsigned char uchar;

//структура с координатами
struct Coordinate_t
{
    float x;						//позиция по x
    float y;						//позиция по y
    float z;						//позицич по z
};

//список координат
typedef vector<Coordinate_t> Coordinates_t;
//список граней
typedef vector<Coordinates_t> Faces_t;
//список ячек, законченных фигур
typedef vector<Faces_t> Figure_t;

//список id типа целое
typedef vector <signed long int> CListInt;
//список множеств
typedef vector <CListInt> CListVariety;
//просто объявление класса клеточных автоматов
class CA;
class CThread;

//список автоматов в клетках
typedef vector<CA *> CListCA;
//список потоков, это же и список разных типов КА, которые работают последовательно
//(друг за другом)
typedef vector<CThread *> CListThread;
typedef vector<float> ClistFloat;

struct CColor
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};
#endif