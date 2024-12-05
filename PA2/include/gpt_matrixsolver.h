#ifndef __GPT_MATRIXSOLVER__H
#define __GPT_MATRIXSOLVER__H
#include <vector>
#include <iostream>

using namespace std;

std::vector<double> conjugateGradient(const std::vector<std::vector<double>>& Q, const std::vector<double>& dx, double tol = 1e-10, int maxIter = 1000);

#endif 