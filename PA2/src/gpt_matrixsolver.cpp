#include <vector>
#include <cmath>
#include <iostream>


// PROGRAM WRITTEN USING COPILOT

// prompt:
//in c++ write code that solves the following matrix calculation: 
//Q * x + dx = 0 Q is a n x n matrix while x and dx are size n. 
//Q and dx are known, the goal is to find x 
//do this with the conjugate gradient method


// Function to perform matrix-vector multiplication O(vec.size()^2)
std::vector<float> matVecMul(const std::vector<std::vector<float>>& matrix, const std::vector<float>& vec) {
    int n = vec.size();
    std::vector<float> result(n, 0.0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i] += matrix[i][j] * vec[j];
        }
    }
    return result;
}

// Function to compute the dot product of two vectors (O(vec1.size()))
float dotProduct(const std::vector<float>& vec1, const std::vector<float>& vec2) {
    float result = 0.0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        result += vec1[i] * vec2[i];
    }
    return result;
}

// Function to add two vectors (O(vec1.size()))
std::vector<float> vecAdd(const std::vector<float>& vec1, const std::vector<float>& vec2) {
    int n = vec1.size();
    std::vector<float> result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = vec1[i] + vec2[i];
    }
    return result;
}

// Function to subtract two vectors (O(vec1.size()))
std::vector<float> vecSub(const std::vector<float>& vec1, const std::vector<float>& vec2) {
    int n = vec1.size();
    std::vector<float> result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = vec1[i] - vec2[i];
    }
    return result;
}

// Function to scale a vector by a scalar O(vec.size())
std::vector<float> vecScale(const std::vector<float>& vec, float scalar) {
    int n = vec.size();
    std::vector<float> result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = vec[i] * scalar;
    }
    return result;
}

// Conjugate Gradient method to solve Q * x + dx = 0
std::vector<float> conjugateGradient(const std::vector<std::vector<float>>& Q, const std::vector<float>& dx, float tol = 1e-10, int maxIter = 1000) {
    int n = dx.size();
    std::vector<float> x(n, 0.0); //declare output vector
    std::vector<float> r = vecScale(dx, -1.0); // multiply dx by -1
    std::vector<float> p = r; 
    float rsold = dotProduct(r, r);

    for (int i = 0; i < maxIter; ++i) {
        std::cout << "Running Iteration " << i << " out of " << maxIter << " Total Iterations" << std::endl;
        std::vector<float> Qp = matVecMul(Q, p);
        float alpha = rsold / dotProduct(p, Qp);
        x = vecAdd(x, vecScale(p, alpha));
        r = vecSub(r, vecScale(Qp, alpha));
        float rsnew = dotProduct(r, r);

        if (sqrt(rsnew) < tol) {
            std::cout << "Finished on Iteration: " << i << std::endl;
            break;
        }

        p = vecAdd(r, vecScale(p, rsnew / rsold));
        rsold = rsnew;
    }

    return x;
}