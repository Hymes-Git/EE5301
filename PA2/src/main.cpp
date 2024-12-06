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
#include <string>

using namespace std;

int circuitWidth, circuitHeight = 0;
/**
 * Using the giving file name, imports the locations to the XVector and YVector variables, used to bypass the gradient descent
 * @param XVector XVector to write x-coordinates to
 * @param YVector YVector to write y-coordinates to
 * @param circuitName name of circuit i.e. ibm01
 */
void importPositionsFromCSV (vector <double> &XVector, vector <double> &YVector, string circuitName);

/**
 * Calculates the total wire length
 * @param qMatrix the q matrix
 * @param XVector vector of x-coordinates for cells
 * @param YVector vector of y-coordinates for cells
 */
double calculateWireLength (vector <vector <double>> &qMatrix, vector <double> &XVector , vector<double> &YVector);

/**
 * Writes the final positions of cells described by the XVector and YVector
 * @param fileName file to write positions to, include the file extension yourself
 * @param XVector passed by reference X positions vector
 * @param YVector passed by reference Y positions vector
 * @param pinLocations passed by reference describes location of pins
 * @param numCells number of non-star movable cells
 * 
 */
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

	string test = inareFileName;

    size_t start = test.find_last_of('/') + 1;  // Find the position after the last '/'
    size_t end = test.find_last_of('.');        // Find the position before the last '.'
	string circuitName = test.substr(start, end - start);  // Extract the substring between '/' and '.'

	// find the name of the circuit i.e. ibm01 or toy02
	cout << circuitName << endl;

	string prespreadFileName = "./positions/positions_prespread_";
	prespreadFileName.append(circuitName);
	prespreadFileName.append(".csv");

	string postspreadFileName = "./positions/positions_postspread_";
	postspreadFileName.append(circuitName);
	postspreadFileName.append(".csv");

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

	bool import = false;

	if (import == true) {
		cout << "Not Performing Conjugate Gradient, Importing Prespread Positions From: " << prespreadFileName << endl;
		importPositionsFromCSV(XVector, YVector, prespreadFileName);
	} else {
		cout << "Running Conjugate Gradient to find XVector" << endl;
		XVector = conjugateGradient(qMatrix, dXVector, 1e-4);
		cout << "Running Conjugate Gradient to find YVector" << endl;
		YVector = conjugateGradient(qMatrix, dYVector, 1e-4);
		writeFinalPositions(prespreadFileName, XVector, YVector, pinLocationsLocal, numCellsAndNoPadsLocal);
	}

	cout << "Pre-spreading Wirelength: " << calculateWireLength(qMatrix, XVector, YVector) << endl;

	XSpreadCells(XSpreadedVector, XVector, YVector, circuitWidth, circuitHeight, 5);
	YSpreadCells(YSpreadedVector, XVector, YVector, circuitWidth, circuitHeight, 5);

	cout << "Post-spreading Wirelength: " << calculateWireLength(qMatrix, XSpreadedVector, YSpreadedVector) << endl << endl;
	writeFinalPositions(postspreadFileName, XSpreadedVector, YSpreadedVector, pinLocationsLocal, numCellsAndNoPadsLocal);
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

void importPositionsFromCSV (vector <double> &XVector, vector <double> &YVector, string circuitName) {
	ifstream file(circuitName);

	if (!file.is_open()) { 
		std::cerr << "Error opening file" << std::endl; 
		return; 
	}

	string line, type;

	int index = 0;
	std::getline(file, line);
	while (std::getline(file, line)) { 
		std::istringstream ss(line); 
		std::string token; // Read the type 
		if (token == "p") {
			break;
		}
		std::getline(ss, type, ','); // Read the x coordinate 
		if (type == "p") {
			break;
		}
		std::getline(ss, token, ','); 
		XVector[index] = std::stod(token);
		std::getline(ss, token, ','); 
		YVector[index] = std::stod(token);
		index += 1;
	}
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