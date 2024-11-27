// written by Suraj Dalvi, ECE Dept., Univ. of Minnesota
// Heavily modified by Kia Bazargan (renaming variables, adding
// comments, creating a clean version of the hyperedge data structure)


#include <iostream>
#include <stdio.h>
#include <map>
#include <vector>
#include <cmath>
#include <string.h>
#include "suraj_parser.h"

using namespace std;

int circuitWidth, circuitHeight = 0;

double calculateGamma(int numEdges);
void populateQMatrixForEdge (vector <vector <double>> &qMatrix, int edge1, int edge2, vector<int> hyperEdgeWeights, int numMovableCells, int numCellsAndPins, int currEdge, int edgeSize);
void initQMatrix(vector <vector <double>> &qMatrix, vector <int> &cellPinArray, vector <int> &hyperEdgestartIndex, vector <int> &hyperEdgeWeights, int numMovableCells, int numCellsAndPins);
void initDXYVectors(vector <double> &dXVector, vector <double> dYVector, vector <int> &cellPinArray, vector <int> &hyperEdgeStartIndexes, vector <int> &hyperEdgeWeights, int numMovableCells, int numCellsAndPins);
double findNetCost(double weight, double xi, double xj, double yi, double yj);
int findNumStars(vector <int> &hyperEdgeStartIndexes);
void initXYVectors (vector <double> &XVector , vector<double> &YVector, vector <SPinLocation> &pinLocations);

int main(int argv, char *argc[])
{
	char inareFileName[100];
	char innetFileName[100];
	char inPadLocationFileName[100];

	if (argv!=2) {
		cout << "Please provide a circuit file name with no extension." << endl;
		return 1;
	}
        	     
    cout << "Reading circuit file " << argc[1] << endl;

	strcpy(inareFileName, argc[1]);
	strcat(inareFileName, ".are");
	strcpy(innetFileName,argc[1]);
	strcat(innetFileName,".net");
	strcpy(inPadLocationFileName,argc[1]);
	strcat(inPadLocationFileName,".kiaPad");

	int success = parseIbmFile(inareFileName, innetFileName, inPadLocationFileName);
	if (success == -1) {
		cout << "Error reading input file(s)" << endl;
		return 0;
	}

	printf("\nNumber of vertices,hyper = %d %d\n",numCellsAndPads,numhyper);

	// transfer data over
	int numCellPinsLocal = numCellPins; // number of all terminals connected to the end points of (hyper) edges
	int numHyperLocal = numhyper; // number of edges and hyperedges
	int numCellsAndPadsLocal = numCellsAndPads; // total number of movable cells (generall with names starting with a), and I/O pads (generally starting with p)
	int numCellsAndNoPadsLocal = numCells_noPads; // total number of movable cells
	int numPins = numCellsAndPadsLocal - numCellsAndNoPadsLocal; // total number of I/O pads or pins

	map <const char *, int, ltstr> nodeNameToNodeNumMapLocal = nodeNameToNodeNum_map;
	vector <int> cellPinArrayLocal;
	vector <int> hyperEdgeIndexToFirstEntryInPinArrayLocal;
	vector <int> hyperWeightsLocal;
	vector <int> vertexSizeLocal;
	vector <SPinLocation> pinLocationsLocal;

	cellPinArrayLocal.resize(numCellPinsLocal);
	hyperEdgeIndexToFirstEntryInPinArrayLocal.resize(numHyperLocal+1);
	hyperWeightsLocal.resize(numHyperLocal);
	vertexSizeLocal.resize(numCellsAndPadsLocal);
	pinLocationsLocal.resize(numPins);

	for (int i = 0; i < numCellPinsLocal; i++) {
		cellPinArrayLocal[i] = cellPinArray[i]; 
	}

	for (int i = 0; i < numHyperLocal+1; i++) {
		hyperEdgeIndexToFirstEntryInPinArrayLocal[i] = hEdge_idxToFirstEntryInPinArray[i];
	}

	for (int i = 0; i < numHyperLocal; i++) {
		hyperWeightsLocal[i] = hyperwts[i];
	}

	for (int i = 0; i < numCellsAndPads; i++) {
		vertexSizeLocal[i] = vertexSize[i];
	}

	for (int i = 0; i < numPins; i++) {
		pinLocationsLocal[i] = pinLocations[i];
	}

	// call function(s) dealing with creating the Q matrix, placement, etc.

	// find number of stars
	int numStars = findNumStars(hyperEdgeIndexToFirstEntryInPinArrayLocal);
	cout << "Num Stars: " << numStars << endl;


	vector <vector <double>> qMatrix;
	vector <double> dXVector;
	vector <double> dYVector; 
	vector <double> XVector;
	vector <double> YVector;
	qMatrix.resize(numCellsAndNoPadsLocal + numStars);
	dXVector.resize(numCellsAndNoPadsLocal + numStars);
	dYVector.resize(numCellsAndNoPadsLocal + numStars);
	XVector.resize(numCellsAndNoPadsLocal + numStars);
	YVector.resize(numCellsAndNoPadsLocal + numStars);
	
	for (int i = 0; i < numCellsAndNoPadsLocal + numStars; i++) {
		qMatrix[i].resize(numCellsAndNoPadsLocal + numStars);
	}

	initQMatrix (qMatrix, cellPinArrayLocal, hyperEdgeIndexToFirstEntryInPinArrayLocal, hyperWeightsLocal, numCellsAndNoPadsLocal, numCellsAndPadsLocal);
	initDXYVectors (dXVector, dYVector, cellPinArrayLocal, hyperEdgeIndexToFirstEntryInPinArrayLocal, hyperWeightsLocal, numCellsAndNoPadsLocal, numCellsAndPadsLocal);
	initXYVectors(XVector, YVector, pinLocationsLocal);

    free(pinLocations);
	free(hEdge_idxToFirstEntryInPinArray);
	free(cellPinArray);
	free(hyperwts);
}

void initQMatrix(vector <vector <double>> &qMatrix, vector <int> &cellPinArray, vector <int> &hyperEdgeStartIndexes, vector <int> &hyperEdgeWeights, int numMovableCells, int numCellsAndPins) {
	int currStar = 0;

	// iterate through each (hyper)edge
	for (unsigned int currEdge = 0; currEdge < hyperEdgeStartIndexes.size()-1; currEdge++) {
		unsigned int startIndex = hyperEdgeStartIndexes[currEdge];
		unsigned int edgeSize = hyperEdgeStartIndexes[currEdge+1] - hyperEdgeStartIndexes[currEdge];

		// edge between 2 nodes (clique)
		if (edgeSize == 2) {
			int edge1 = cellPinArray[startIndex];
			int edge2 = cellPinArray[startIndex+1];

			populateQMatrixForEdge(qMatrix, edge1, edge2, hyperEdgeWeights, numMovableCells, numCellsAndPins, currEdge, edgeSize);
		}

		// edge between 3 nodes (clique)
		else if (edgeSize == 3) {
			int edge1, edge2;
			
			for (int side = 0; side < 3; side++) {
				if (side == 0) {
					edge1 = cellPinArray[startIndex];
					edge2 = cellPinArray[startIndex+1]; 
				} else if (side == 1) {
					edge1 = cellPinArray[startIndex+1];
					edge2 = cellPinArray[startIndex+2];
 				} else {
					edge1 = cellPinArray[startIndex];
					edge2 = cellPinArray[startIndex+2];
				}

				populateQMatrixForEdge(qMatrix, edge1, edge2, hyperEdgeWeights, numMovableCells, numCellsAndPins, currEdge, edgeSize);
			}

		}

		// edge between 4+ nodes (star)
		else {
			int edge1, edge2;
			vector <int> nodesInStar;
			nodesInStar.resize(edgeSize);

			for (unsigned int node = 0; node < edgeSize; node++) {
				nodesInStar[node] = cellPinArray[startIndex + node];
			}

			edge2 = numCellsAndPins + currStar; // edge 2 is the current number star its going through
			currStar += 1;

			for (unsigned int node = 0; node < edgeSize; node++) {
				edge1 = nodesInStar[node];
				populateQMatrixForEdge(qMatrix, edge1, edge2, hyperEdgeWeights, numMovableCells, numCellsAndPins, currEdge, edgeSize);
			}



		}

	}

}

double calculateGamma(int numEdges) {
	double gamma;
	
	if (numEdges > 3) {
		// is a star
		gamma = numEdges / (numEdges - 1.0);
	} else {
		// is a clique
		gamma = 1.0 / (numEdges - 1.0);
	}

	return gamma;

}

void populateQMatrixForEdge (vector <vector <double>> &qMatrix, int edge1, int edge2, vector<int> hyperEdgeWeights, int numMovableCells, int numCellsAndPins, int currEdge, int edgeSize) {
	int numPins = numCellsAndPins - numMovableCells;
	if (edge1 >= numMovableCells && edge2 >= numMovableCells && edge1 < numCellsAndPins && edge2 < numCellsAndPins) {
		// edge1 and edge2 are pins, but not star nodes, do nothing, unsure if this happens in reality, but don't want to take any chances				
	} else if (edge1 >= numMovableCells && edge1 < numCellsAndPins) {
		// edge1 is pin, edge2 is cell
		if (edge2 >= numCellsAndPins) // edge 2 is star node, get it back in index
			edge2 -= numPins;
		qMatrix[edge2][edge2] += hyperEdgeWeights[currEdge] * calculateGamma(edgeSize);
	} else if (edge2 >= numMovableCells && edge2 < numCellsAndPins) {
		// edge1 is cell, edge2 is pin
		if (edge1 >= numCellsAndPins) // edge1 is star node, get it back in index
			edge1 -= numPins;
		qMatrix[edge1][edge1] += hyperEdgeWeights[currEdge] * calculateGamma(edgeSize);
	} else {
		// edge between 2 cells
		if (edge1 >= numCellsAndPins)
			edge1 -= numPins;
		if (edge2 >= numCellsAndPins)
			edge2 -= numPins;					
		qMatrix[edge1][edge1] += hyperEdgeWeights[currEdge] * calculateGamma(edgeSize);
		qMatrix[edge2][edge2] += hyperEdgeWeights[currEdge] * calculateGamma(edgeSize);
		qMatrix[edge1][edge2] -= hyperEdgeWeights[currEdge] * calculateGamma(edgeSize);
		qMatrix[edge2][edge1] -= hyperEdgeWeights[currEdge] * calculateGamma(edgeSize);
	}
}

void initDXYVectors(vector <double> &dXVector, vector <double> dYVector, vector <int> &cellPinArray, vector <int> &hyperEdgestartIndex, vector <int> &hyperEdgeWeights, int numMovableCells, int numCellsAndPins) {

}

double findNetCost(double weight, double xi, double xj, double yi, double yj) {
	double cost = 0;

	cost = (0.5) * weight * (  pow((xi - xj), 2.0) + pow((yi - yj), 2.0)  );

	return cost;

}

int findNumStars(vector <int> &hyperEdgeStartIndexes) {
	int numStars = 0;
	int lastEdgeIndex = 0;
	int currentEdgeIndex = 0;
	for (unsigned int index = 0; index < hyperEdgeStartIndexes.size(); index++) {
		currentEdgeIndex = hyperEdgeStartIndexes[index];
		if (currentEdgeIndex - lastEdgeIndex > 3) {
			numStars += 1;
		}
		lastEdgeIndex = currentEdgeIndex;
	}

	return numStars;
}

void initXYVectors (vector <double> &XVector , vector<double> &YVector, vector <SPinLocation> &pinLocations) {

	for (unsigned int pinNum = 0; pinNum < pinLocations.size(); pinNum++) {
		if (pinLocations[pinNum].x > circuitWidth) {
			circuitWidth = pinLocations[pinNum].x;
		}

		if (pinLocations[pinNum].y > circuitHeight) {
			circuitHeight = pinLocations[pinNum].y;
		}
	}

	for (unsigned int cellNum = 0; cellNum < XVector.size(); cellNum++) {
		XVector[cellNum] = circuitWidth / 2.0;
		YVector[cellNum] = circuitHeight / 2.0;
	}

}