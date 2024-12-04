#include <iostream>
#include <vector>
#include <cmath>

// PROGRAM WRITTEN USING COPILOT

// prompt:
//in c++ write code that solves the following matrix calculation: 
//Q * x + dx = 0 Q is a n x n matrix while x and dx are size n. 
//Q and dx are known, the goal is to find x 
//do this with the conjugate gradient method


// Function to perform matrix-vector multiplication
std::vector<double> matVecMul(const std::vector<std::vector<double>>& matrix, const std::vector<double>& vec) {
    int n = vec.size();
    std::vector<double> result(n, 0.0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i] += matrix[i][j] * vec[j];
        }
    }
    return result;
}

// Function to compute the dot product of two vectors
double dotProduct(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    double result = 0.0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        result += vec1[i] * vec2[i];
    }
    return result;
}

// Function to add two vectors
std::vector<double> vecAdd(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    int n = vec1.size();
    std::vector<double> result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = vec1[i] + vec2[i];
    }
    return result;
}

// Function to subtract two vectors
std::vector<double> vecSub(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    int n = vec1.size();
    std::vector<double> result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = vec1[i] - vec2[i];
    }
    return result;
}

// Function to scale a vector by a scalar
std::vector<double> vecScale(const std::vector<double>& vec, double scalar) {
    int n = vec.size();
    std::vector<double> result(n);
    for (int i = 0; i < n; ++i) {
        result[i] = vec[i] * scalar;
    }
    return result;
}

// Conjugate Gradient method to solve Q * x + dx = 0
std::vector<double> conjugateGradient(const std::vector<std::vector<double>>& Q, const std::vector<double>& dx, double tol = 1e-10, int maxIter = 1000) {
    int n = dx.size();
    std::vector<double> x(n, 0.0);
    std::vector<double> r = vecScale(dx, -1.0);
    std::vector<double> p = r;
    double rsold = dotProduct(r, r);

    for (int i = 0; i < maxIter; ++i) {
        std::vector<double> Qp = matVecMul(Q, p);
        double alpha = rsold / dotProduct(p, Qp);
        x = vecAdd(x, vecScale(p, alpha));
        r = vecSub(r, vecScale(Qp, alpha));
        double rsnew = dotProduct(r, r);

        if (sqrt(rsnew) < tol) {
            break;
        }

        p = vecAdd(r, vecScale(p, rsnew / rsold));
        rsold = rsnew;
    }

    return x;
}

// int main() {
//     // Example input
//     std::vector<std::vector<double>> Q = {
//         {4, 1},
//         {1, 3}
//     };

//     std::vector<double> dx = {1, 2};

//     // Solve the matrix equation using the Conjugate Gradient method
//     std::vector<double> x = conjugateGradient(Q, dx);

//     // Output the result
//     std::cout << "Solution x: ";
//     for (const double& val : x) {
//         std::cout << val << " ";
//     }
//     std::cout << std::endl;

//     return 0;
// }
