#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <map>
#include <vector>
#include <cmath>
#include <string.h>
#include "suraj_parser.h"
#include "gpt_matrixsolver.h"
#include "QDXDYInitializer.h"
#include "cellSpreader.h"

using namespace std;

int circuitWidth, circuitHeight = 0;

double calculateWireLength (vector <vector <double>> &qMatrix, vector <double> &XVector , vector<double> &YVector);
void writeFinalPositions(string fileName, vector <double> &XVector, vector <double> &YVector, vector <SPinLocation> &pinLocations, int numCells);

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

    free(pinLocations);
	free(hEdge_idxToFirstEntryInPinArray);
	free(cellPinArray);
	free(hyperwts);

	// call function(s) dealing with creating the Q matrix, placement, etc.

	// find number of stars
	int numStars = findNumStars(hyperEdgeIndexToFirstEntryInPinArrayLocal);
	cout << "Num Stars: " << numStars << endl;

	// create vectors and resize them
	vector <vector <double>> qMatrix;
	vector <double> dXVector;
	vector <double> dYVector; 
	vector <double> XVector;
	vector <double> YVector;
	vector <double> XSpreadedVector;
	vector <double> YSpreadedVector;
	qMatrix.resize(numCellsAndNoPadsLocal + numStars);
	dXVector.resize(numCellsAndNoPadsLocal + numStars);
	dYVector.resize(numCellsAndNoPadsLocal + numStars);
	XVector.resize(numCellsAndNoPadsLocal + numStars);
	YVector.resize(numCellsAndNoPadsLocal + numStars);
	XSpreadedVector.resize(numCellsAndNoPadsLocal + numStars);
	YSpreadedVector.resize(numCellsAndNoPadsLocal + numStars);
	
	for (int i = 0; i < numCellsAndNoPadsLocal + numStars; i++) {
		qMatrix[i].resize(numCellsAndNoPadsLocal + numStars);
	}

	// initialize q matrix and vectors
	initQMatrix (qMatrix, cellPinArrayLocal, hyperEdgeIndexToFirstEntryInPinArrayLocal, hyperWeightsLocal, numCellsAndNoPadsLocal, numCellsAndPadsLocal);
	initDXYVectors (dXVector, dYVector, pinLocationsLocal, cellPinArrayLocal, hyperEdgeIndexToFirstEntryInPinArrayLocal, hyperWeightsLocal, numCellsAndNoPadsLocal, numCellsAndPadsLocal);
	initXYVectors(XVector, YVector, pinLocationsLocal, circuitWidth, circuitHeight);

	XVector = conjugateGradient(qMatrix, dXVector);
	YVector = conjugateGradient(qMatrix, dYVector);

	cout << "Pre-spreading Wirelength: " << calculateWireLength(qMatrix, XVector, YVector) << endl;
	writeFinalPositions("positions_prespread.csv", XVector, YVector, pinLocationsLocal, numCellsAndNoPadsLocal);

	XSpreadCells(XSpreadedVector, XVector, YVector, circuitWidth, circuitHeight, 5);
	YSpreadCells(YSpreadedVector, XVector, YVector, circuitWidth, circuitHeight, 5);

	cout << "Post-spreading Wirelength: " << calculateWireLength(qMatrix, XSpreadedVector, YSpreadedVector) << endl << endl;
	writeFinalPositions("positions_postspread.csv", XSpreadedVector, YSpreadedVector, pinLocationsLocal, numCellsAndNoPadsLocal);
}

double calculateWireLength (vector <vector <double>> &qMatrix, vector <double> &XVector , vector<double> &YVector) {
	double sum = 0;
	int numNodes = qMatrix.size();
	for (int row = 0; row < numNodes; row++) {
		for (int column = 0; column < numNodes; column++) {

			double entry = qMatrix[row][column];

			if (entry < 0) {
				sum += -1 * entry * ( pow((XVector[row] - XVector[column]), 2.0) + pow((YVector[row] - YVector[column]), 2.0) );
			}

		}
	}

	sum = sqrt(sum);
	return sum;

}

void writeFinalPositions(string fileName, vector <double> &XVector, vector <double> &YVector, vector <SPinLocation> &pinLocations, int numCells) {

	ofstream fileOut;

	int numEntries = XVector.size();
	int numPins = pinLocations.size();

	fileOut.open(fileName);
    
    if (!fileOut.is_open()) {
        cout << "ERROR: Unable to open file: " << fileName << endl;
        return;
	}

	ostringstream output;

	output << "type, x coordinate, y coordinate" << endl;

	for (int entry = 0; entry < numEntries; entry++) {

		if (entry < numCells) {
			output << "a, " << XVector[entry] << ", " << YVector[entry] << endl;
		} else {
			// is a star node 
			output << "s, " << XVector[entry] << ", " << YVector[entry] << endl;
		}

	}

	for (int entry = 0; entry < numPins; entry++) {
		output << "p, " << pinLocations[entry].x << ", " << pinLocations[entry].y << endl;
	}

	fileOut << output.str();
	fileOut.close();

}