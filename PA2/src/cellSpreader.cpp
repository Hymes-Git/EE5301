#include "cellSpreader.h"
#include <iostream>

vector < vector <vector <int>>> initBinMatrix(vector <double> &XVector, vector <double> &YVector, int circuitWidth, int circuitHeight, int k) {
    int numCells = XVector.size();

    vector < vector <vector <int>>> binMatrix;
    vector <double> OBX;
    vector <double> OBY;
    OBX.resize(k);
    OBY.resize(k);

    OBX[0] = 0;
    OBY[0] = 0;
    for (int i = 1; i < k; i++) {
        OBX[i] = OBX[i-1] + (circuitWidth / k);
        OBY[i] = OBY[i-1] + (circuitHeight / k);
    }

    binMatrix.resize(k);
    for (int i = 0; i < k; i++) {
        binMatrix[i].resize(k);
    }

    for (int cell = 0; cell < numCells; cell++) {
        double XCord = XVector[cell];
        double YCord = YVector[cell];
        int xIndex = 0;
        int yIndex = 0;

        for (int i = 0; i < k; i++) {
            if (XCord >= OBX[i] && XCord < OBX[i+1]) {
                xIndex = i;
            }
        }

        if (XCord >= OBX[k-1]) {
            xIndex = k-1;
        }

        for (int i =0; i < k; i++) {
            if (YCord >= OBY[i] && YCord < OBY[i+1]) {
                yIndex = i;
            }            
        }

        if (YCord >= OBY[k-1]) {
            yIndex = k-1;
        }

        binMatrix[yIndex][xIndex].push_back(cell);

    }  

    return binMatrix; 

}

void XSpreadCells(vector <double> &XSpreadedVector, vector <double> &XVector, vector <double> &YVector, int circuitWidth, int circuitHeight, int k) { 
    vector < vector <vector <int>>> binMatrix = initBinMatrix(XVector, YVector, circuitWidth, circuitHeight, k);
    vector <double> OB;
    vector <double> NB;
    OB.resize(k);
    NB.resize(k);

    OB[0] = 0;
    for (int i = 1; i < k; i++) {
        OB[i] = OB[i-1] + (circuitWidth / k);
    }

    XSpreadedVector = XVector; 

}

void YSpreadCells(vector <double> &YSpreadedVector, vector <double> &XVector, vector <double> &YVector, int circuitWidth, int circuitHeight, int k) {
    vector < vector <vector <int>>> binMatrix = initBinMatrix(XVector, YVector, circuitWidth, circuitHeight, k);
    vector <double> OB;
    vector <double> NB;
    OB.resize(k);
    NB.resize(k);

    OB[0] = 0;
    for (int i = 1; i < k; i++) {
        OB[i] = OB[i-1] + (circuitHeight / k);
    }
    YSpreadedVector = YVector; 

}