// written by Suraj Dalvi, ECE Dept., Univ. of Minnesota
// Heavily modified by Kia Bazargan (renaming variables, adding
// comments, creating a clean version of the hyperedge data structure)

#ifndef __GPT_MATRIXSOLVER__H
#define __GPT_MATRIXSOLVER__H
#include <vector>

using namespace std;


std::vector<std::vector<double>> inverse(const std::vector<std::vector<double>>& matrix);
std::vector<double> multiply(const std::vector<std::vector<double>>& matrix, const std::vector<double>& vector);
std::vector <double> computePositions(vector <vector <double>> qMatrix, vector <double> dx);


#endif 