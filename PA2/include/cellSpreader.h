#ifndef __CELLSPREADER__H
#define __CELLSPREADER__H
#include <vector>
#include "suraj_parser.h"

using namespace std;

vector < vector <vector <int>>> initBinMatrix(vector <float> &XVector, vector <float> &YVector, int circuitWidth, int circuitHeight, int k);
void XSpreadCells(vector <float> &XSpreadedVector, vector <float> &XVector, vector <float> &YVector, int circuitWidth, int circuitHeight, int k);
void YSpreadCells(vector <float> &YSpreadedVector, vector <float> &XVector, vector <float> &YVector, int circuitWidth, int circuitHeight, int k);

#endif 