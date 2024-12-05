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
    vector <double> U;
    double delta = 1.5;
    double alpha = 0.8;
    OB.resize(k+1);
    NB.resize(k+1);
    U.resize(k+1);

    NB[0] = 0;
    OB[k] = circuitWidth;


    OB[0] = 0;
    for (int i = 1; i < k; i++) {
        OB[i] = OB[i-1] + (circuitWidth / k);
    }

    // row
    for (int row = 0; row < k; row++) {

        for (int column = 0; column < k; column++) {
            U[column+1] = binMatrix[row][column].size();
        }

        for (int column = 1; column < k; column++) {
            NB[column] = ( OB[column-1] * (U[column+1] + delta) + OB[column+1] * (U[column] + delta) )
                        / (U[column] + U[column+1] + 2*delta);
        }

        for (int column = 1; column < k; column++) {
            
            vector <int> cellList = binMatrix[row][column];
            
            for (unsigned int cellNum = 0; cellNum < cellList.size(); cellNum++) {
                int cell = cellList[cellNum];

                XSpreadedVector[cell] = ( NB[column] * (XVector[cell] - OB[column-1]) + NB[column-1] * (OB[column] - XVector[cell]) )
                                        / (OB[column] - OB[column-1]);   
            }

        }

    }

    for (unsigned int cellNum = 0; cellNum < XSpreadedVector.size(); cellNum++) {

        double xj = XVector[cellNum];
        double xjprime = XSpreadedVector[cellNum];
        XSpreadedVector[cellNum] = xj + alpha * (xjprime - xj);

    }

   // XSpreadedVector = XVector; 

}

void YSpreadCells(vector <double> &YSpreadedVector, vector <double> &XVector, vector <double> &YVector, int circuitWidth, int circuitHeight, int k) {
    vector < vector <vector <int>>> binMatrix = initBinMatrix(XVector, YVector, circuitWidth, circuitHeight, k);
    vector <double> OB;
    vector <double> NB;
    vector <double> U;
    double delta = 1.5;
    double alpha = 0.8;
    OB.resize(k+1);
    NB.resize(k+1);
    U.resize(k+1);

    NB[0] = 0;
    OB[k] = circuitHeight;

    OB[0] = 0;
    for (int i = 1; i < k; i++) {
        OB[i] = OB[i-1] + (circuitHeight / k);
    }

    // row
    for (int column = 0; column < k; column++) {

        for (int row = 0; row < k; row++) {
            U[row+1] = binMatrix[row][column].size();
        }

        for (int row = 1; row < k; row++) {
            NB[row] = ( OB[row-1] * (U[row+1] + delta) + OB[row+1] * (U[row] + delta) )
                        / (U[row] + U[row+1] + 2*delta);
        }

        for (int row = 1; row < k; row++) {
            
            vector <int> cellList = binMatrix[row][column];
            
            for (unsigned int cellNum = 0; cellNum < cellList.size(); cellNum++) {
                int cell = cellList[cellNum];

                YSpreadedVector[cell] = ( NB[row] * (YVector[cell] - OB[row-1]) + NB[row-1] * (OB[row] - YVector[cell]) )
                                        / (OB[row] - OB[row-1]);   
            }

        }

    }

    for (unsigned int cellNum = 0; cellNum < YSpreadedVector.size(); cellNum++) {

        double yj = YVector[cellNum];
        double yjprime = YSpreadedVector[cellNum];
        YSpreadedVector[cellNum] = yj + alpha * (yjprime - yj);

    }

    // YSpreadedVector = YVector;

}