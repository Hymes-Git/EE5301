#ifndef __CELLSPREADER__H
#define __CELLSPREADER__H
#include <vector>
#include "suraj_parser.h"

using namespace std;

vector < vector <vector <int>>> initBinMatrix(vector <double> &XVector, vector <double> &YVector, int circuitWidth, int circuitHeight, int k);
void XSpreadCells(vector <double> &XSpreadedVector, vector <double> &XVector, vector <double> &YVector, int circuitWidth, int circuitHeight, int k);
void YSpreadCells(vector <double> &YSpreadedVector, vector <double> &XVector, vector <double> &YVector, int circuitWidth, int circuitHeight, int k);

#endif 