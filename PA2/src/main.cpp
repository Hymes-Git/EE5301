// written by Suraj Dalvi, ECE Dept., Univ. of Minnesota
// Heavily modified by Kia Bazargan (renaming variables, adding
// comments, creating a clean version of the hyperedge data structure)


#include<iostream>
#include<stdio.h>
#include<map>
#include<vector>
#include <string.h>
#include "suraj_parser.h"

using namespace std;

int circuitWidth, circuitHeight = 0;
	
int findNumStars(vector <int> hyperEdgeIndexes);
void initDVectors (vector <double> &dXVector , vector<double> &dYVector, vector <SPinLocation> &pinLocations);

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
	qMatrix.resize(numCellsAndNoPadsLocal + numStars);
	dXVector.resize(numCellsAndNoPadsLocal + numStars);
	dYVector.resize(numCellsAndNoPadsLocal + numStars);
	
	for (int i = 0; i < numCellsAndNoPadsLocal + numStars; i++) {
		qMatrix[i].resize(numCellsAndNoPadsLocal + numStars);
	}

	initDVectors(dXVector, dYVector, pinLocationsLocal);



    free(pinLocations);
	free(hEdge_idxToFirstEntryInPinArray);
	free(cellPinArray);
	free(hyperwts);
}

int findNumStars(vector <int> hyperEdgeIndexes) {
	int numStars = 0;
	int lastEdgeIndex = 0;
	int currentEdgeIndex = 0;
	for (unsigned int index = 0; index < hyperEdgeIndexes.size(); index++) {
		currentEdgeIndex = hyperEdgeIndexes[index];
		if (currentEdgeIndex - lastEdgeIndex > 3) {
			numStars += 1;
		}
		lastEdgeIndex = currentEdgeIndex;
	}

	return numStars;
}

void initDVectors (vector <double> &dXVector , vector<double> &dYVector, vector <SPinLocation> &pinLocations) {

	for (unsigned int pinNum = 0; pinNum < pinLocations.size(); pinNum++) {
		if (pinLocations[pinNum].x > circuitWidth) {
			circuitWidth = pinLocations[pinNum].x;
		}

		if (pinLocations[pinNum].y > circuitHeight) {
			circuitHeight = pinLocations[pinNum].y;
		}
	}

	for (unsigned int cellNum = 0; cellNum < dXVector.size(); cellNum++) {
		dXVector[cellNum] = circuitWidth / 2.0;
		dYVector[cellNum] = circuitHeight / 2.0;
	}

}