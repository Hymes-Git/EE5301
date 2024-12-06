#ifndef __QDXDYINITIALIZER__H
#define __QDXDYINITIALIZER__H
#include <vector>
#include "suraj_parser.h"

using namespace std;


float calculateGamma (int numEdges);
void populateQMatrixForEdge (vector <vector <float>> &qMatrix, int edge1, int edge2, vector<int> hyperEdgeWeights, int numMovableCells, int numCellsAndPins, int currEdge, int edgeSize);
void initQMatrix(vector <vector <float>> &qMatrix, vector <int> &cellPinArray, vector <int> &hyperEdgestartIndexes, vector <int> &hyperEdgeWeights, int numMovableCells, int numCellsAndPins);
void populateDXYVectorsForEdge (vector <float> &dXVector, vector <float> &dYVector, vector <SPinLocation> &pinLocations, int edge1, int edge2, vector<int> hyperEdgeWeights, int numMovableCells, int numCellsAndPins, int currEdge, int edgeSize);
void initDXYVectors(vector <float> &dXVector, vector <float> &dYVector, vector <SPinLocation> &pinLocations, vector <int> &cellPinArray, vector <int> &hyperEdgeStartIndexes, vector <int> &hyperEdgeWeights, int numMovableCells, int numCellsAndPins);
int findNumStars(vector <int> &hyperEdgeStartIndexes);
void initXYVectors (vector <float> &XVector , vector<float> &YVector, vector <SPinLocation> &pinLocations, int &circuitWidth, int &circuitHeight);

#endif 