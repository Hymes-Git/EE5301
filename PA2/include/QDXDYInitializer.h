#ifndef __QDXDYINITIALIZER__H
#define __QDXDYINITIALIZER__H
#include <vector>
#include "suraj_parser.h"

using namespace std;


double calculateGamma(int numEdges);
void populateQMatrixForEdge (vector <vector <double>> &qMatrix, int edge1, int edge2, vector<int> hyperEdgeWeights, int numMovableCells, int numCellsAndPins, int currEdge, int edgeSize);
void initQMatrix(vector <vector <double>> &qMatrix, vector <int> &cellPinArray, vector <int> &hyperEdgestartIndexes, vector <int> &hyperEdgeWeights, int numMovableCells, int numCellsAndPins);
void populateDXYVectorsForEdge (vector <double> &dXVector, vector <double> &dYVector, vector <SPinLocation> &pinLocations, int edge1, int edge2, vector<int> hyperEdgeWeights, int numMovableCells, int numCellsAndPins, int currEdge, int edgeSize);
void initDXYVectors(vector <double> &dXVector, vector <double> &dYVector, vector <SPinLocation> &pinLocations, vector <int> &cellPinArray, vector <int> &hyperEdgeStartIndexes, vector <int> &hyperEdgeWeights, int numMovableCells, int numCellsAndPins);
int findNumStars(vector <int> &hyperEdgeStartIndexes);
void initXYVectors (vector <double> &XVector , vector<double> &YVector, vector <SPinLocation> &pinLocations, int circuitWidth, int circuitHeight);

#endif 