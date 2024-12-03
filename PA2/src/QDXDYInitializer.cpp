#include "QDXDYInitializer.h"

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

void populateDXYVectorsForEdge (vector <double> &dXVector, vector <double> &dYVector, vector <SPinLocation> &pinLocations, int edge1, int edge2, vector<int> hyperEdgeWeights, int numMovableCells, int numCellsAndPins, int currEdge, int edgeSize) {
	int numPins = numCellsAndPins - numMovableCells;
	if (edge1 >= numMovableCells && edge2 >= numMovableCells && edge1 < numCellsAndPins && edge2 < numCellsAndPins) {
		// edge1 and edge2 are pins, but not star nodes, do nothing, unsure if this happens in reality, but don't want to take any chances				
	} else if (edge1 >= numMovableCells && edge1 < numCellsAndPins) {
		// edge1 is pin, edge2 is cell
		if (edge2 >= numCellsAndPins) // edge 2 is star node, get it back in index
			edge2 -= numPins;
		dXVector[edge2] -= hyperEdgeWeights[currEdge] * calculateGamma(edgeSize) * pinLocations[edge1 - numMovableCells].x;
		dYVector[edge2] -= hyperEdgeWeights[currEdge] * calculateGamma(edgeSize) * pinLocations[edge1 - numMovableCells].y;
	} else if (edge2 >= numMovableCells && edge2 < numCellsAndPins) {
		// edge1 is cell, edge2 is pin
		if (edge1 >= numCellsAndPins) // edge1 is star node, get it back in index
			edge1 -= numPins;
		dXVector[edge1] -= hyperEdgeWeights[currEdge] * calculateGamma(edgeSize) * pinLocations[edge2 - numMovableCells].x;
		dYVector[edge1] -= hyperEdgeWeights[currEdge] * calculateGamma(edgeSize) * pinLocations[edge2 - numMovableCells].y;
	} else {
		// edge between 2 cells, nothing to do
	}	
}

void initDXYVectors(vector <double> &dXVector, vector <double> &dYVector, vector <SPinLocation> &pinLocations, vector <int> &cellPinArray, vector <int> &hyperEdgeStartIndexes, vector <int> &hyperEdgeWeights, int numMovableCells, int numCellsAndPins) {
	int currStar = 0;

	// iterate through each (hyper)edge
	for (unsigned int currEdge = 0; currEdge < hyperEdgeStartIndexes.size()-1; currEdge++) {
		unsigned int startIndex = hyperEdgeStartIndexes[currEdge];
		unsigned int edgeSize = hyperEdgeStartIndexes[currEdge+1] - hyperEdgeStartIndexes[currEdge];

		// edge between 2 nodes (clique)
		if (edgeSize == 2) {
			int edge1 = cellPinArray[startIndex];
			int edge2 = cellPinArray[startIndex+1];

			populateDXYVectorsForEdge(dXVector, dYVector, pinLocations, edge1, edge2, hyperEdgeWeights, numMovableCells, numCellsAndPins, currEdge, edgeSize);
			
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

				populateDXYVectorsForEdge(dXVector, dYVector, pinLocations, edge1, edge2, hyperEdgeWeights, numMovableCells, numCellsAndPins, currEdge, edgeSize);
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
				populateDXYVectorsForEdge(dXVector, dYVector, pinLocations, edge1, edge2, hyperEdgeWeights, numMovableCells, numCellsAndPins, currEdge, edgeSize);
			}



		}

	}

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

void initXYVectors (vector <double> &XVector , vector<double> &YVector, vector <SPinLocation> &pinLocations, int circuitWidth, int circuitHeight) {

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