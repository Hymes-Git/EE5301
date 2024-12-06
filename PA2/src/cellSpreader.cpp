#include "cellSpreader.h"
#include <iostream>
#include <assert.h>

vector < vector <vector <int>>> initBinMatrix(vector <float> &XVector, vector <float> &YVector, int circuitWidth, int circuitHeight, int k) {
    int numCells = XVector.size();

    vector < vector <vector <int>>> binMatrix;
    vector <float> OBX;
    vector <float> OBY;
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
        float XCord = XVector[cell];
        float YCord = YVector[cell];
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

void XSpreadCells(vector <float> &XSpreadedVector, vector <float> &XVector, vector <float> &YVector, int circuitWidth, int circuitHeight, int k) { 
    vector < vector <vector <int>>> binMatrix = initBinMatrix(XVector, YVector, circuitWidth, circuitHeight, k);
    vector <float> OB;
    vector <float> NB;
    vector <float> U;
    float delta = 1.5;
    float alpha = 0.8;
    OB.resize(k+1);
    NB.resize(k+1);
    U.resize(k+1);

    NB[0] = 0;
    OB[k] = circuitWidth;
    NB[k] = circuitWidth;


    OB[0] = 0;
    for (int i = 1; i < k; i++) {
        OB[i] = OB[i-1] + (circuitWidth / (float)k);
    }

    // row
    for (int row = 0; row < k; row++) {

        //cout << "Row: " << row << endl;

        // Calculate U Values for given row
        for (int column = 0; column < k; column++) {
            U[column] = binMatrix[row][column].size();
        }

        // Calculate NB Values for given row
        for (int column = 1; column < k; column++) {
            float topLeft = OB[column-1] * (U[column] + delta);
            float topRight = OB[column+1] * (U[column-1] + delta);
            float bottom = U[column-1] + U[column] + 2.0 * delta;
            NB[column] = (topLeft + topRight) / bottom;


            // cout << "Error Found Printing All Relevant Values" << endl;
            // cout << "U[i] " << U[column] << " ";            
            // cout << "U[i+1] " << U[column+1] << " ";
            // cout << "OB[i-1] " << OB[column-1] << " ";
            // cout << "OB[i+1] " << OB[column+1] << endl;            

 

            assert(NB[column <= circuitWidth]);
        }

        // for (unsigned int i = 0; i < NB.size(); i++) {
        //     cout << "Column: " << i << " " << OB[i] << " " << NB[i] << " " << U[i] << endl;
        // }

        // go through each column
        for (int column = 1; column < k-1; column++) {
            
            vector <int> cellList = binMatrix[row][column];

            // cout << row << " " << column << endl;
            
            // for given column calculate all spreaded x coordinates
            for (unsigned int cellNum = 0; cellNum < cellList.size(); cellNum++) {
                int cell = cellList[cellNum];

                float topLeft = NB[column+1] * (XVector[cell] - OB[column]);
                float topRight = NB[column] * (OB[column+1]-XVector[cell]);
                float bottom = OB[column+1] - OB[column];
                XSpreadedVector[cell] = (topLeft+topRight) / bottom;

                // run the alpha scaler
                float xj = XVector[cell];
                float xjprime = XSpreadedVector[cell];
                XSpreadedVector[cell] = xj + alpha * (xjprime - xj);

                if (XSpreadedVector[cell] > circuitWidth) {
                    cout << "XSpreadedVector: " << XSpreadedVector[cell] << endl;
                    cout << "cell: " << cell << endl;
                    cout << "Error Found Printing All Relevant Values" << endl;
                    cout << "NB[i+1]" << NB[column+1] << " ";
                    cout << "X[i]" << XVector[cell] << " ";
                    cout << "OB[i]" << OB[column] << " ";
                    cout << "NB[i]" << NB[column] << " ";
                    cout << "OB[i+1]" << OB[column+1] << endl;
                }

                assert(XSpreadedVector[cell] <= circuitWidth); 
            }

        }

    }

   // XSpreadedVector = XVector; 

}

void YSpreadCells(vector <float> &YSpreadedVector, vector <float> &XVector, vector <float> &YVector, int circuitWidth, int circuitHeight, int k) {
    vector < vector <vector <int>>> binMatrix = initBinMatrix(XVector, YVector, circuitWidth, circuitHeight, k);
    vector <float> OB;
    vector <float> NB;
    vector <float> U;
    float delta = 1.5;
    float alpha = 0.8;
    OB.resize(k+1);
    NB.resize(k+1);
    U.resize(k+1);

    NB[0] = 0;
    OB[k] = circuitHeight;

    OB[0] = 0;
    for (int i = 1; i < k; i++) {
        OB[i] = OB[i-1] + (circuitHeight / k);
    }

    // Go through each column
    for (int column = 0; column < k; column++) {

        // calculate all U values for given column
        for (int row = 0; row < k; row++) {
            U[row] = binMatrix[row][column].size();
        }

        // calculate all NB values for given column
        for (int row = 1; row < k; row++) {
            float topLeft = OB[row-1] * (U[row] + delta);
            float topRight = OB[row+1] * (U[row-1] + delta);
            float bottom = U[row-1] + U[row] + 2.0 * delta;
            NB[row] = (topLeft + topRight) / bottom;

            assert(NB[row] <= circuitHeight); 

        }

        for (int row = 1; row < k; row++) {
            
            vector <int> cellList = binMatrix[row][column];
            
            for (unsigned int cellNum = 0; cellNum < cellList.size(); cellNum++) {
                int cell = cellList[cellNum];

                float topLeft = NB[row+1] * (YVector[cell] - OB[row]);
                float topRight = NB[row] * (OB[row+1]-YVector[cell]);
                float bottom = OB[row+1] - OB[row];
                YSpreadedVector[cell] = (topLeft+topRight) / bottom;         

                float yj = YVector[cell];
                float yjprime = YSpreadedVector[cell];
                YSpreadedVector[cell] = yj + alpha * (yjprime - yj);                              

                if (YSpreadedVector[cell] > circuitHeight) {
                    cout << "Top Left, Right, Bottom: " << topLeft << " " << topRight << " " << bottom << endl;
                    cout << "YSpreadedVector: " << YSpreadedVector[cell] << endl;
                    cout << "cell: " << cell << endl;
                    cout << "Error Found Printing All Relevant Values" << endl;
                    cout << "NB[i+1]" << NB[row+1] << " ";
                    cout << "Y[i]" << YVector[cell] << " ";
                    cout << "OB[i]" << OB[row] << " ";
                    cout << "NB[i]" << NB[row] << " ";
                    cout << "OB[i+1]" << OB[row+1] << endl;
                }

                assert(YSpreadedVector[cell] <= circuitHeight); 

            }

        }

    }

}