#ifndef __GPT_MATRIXSOLVER__H
#define __GPT_MATRIXSOLVER__H
#include <vector>
#include <iostream>

using namespace std;

std::vector<float> conjugateGradient(const std::vector<std::vector<float>>& Q, const std::vector<float>& dx, float tol = 1e-10, int maxIter = 1000);

#endif 