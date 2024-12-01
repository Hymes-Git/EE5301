#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

using namespace std;

// Function to compute the inverse of a matrix
std::vector<std::vector<double>> inverse(const std::vector<std::vector<double>>& matrix) {
    int n = matrix.size();
    std::vector<std::vector<double>> augmented(matrix);

    // Create an augmented matrix with the identity matrix
    for (int i = 0; i < n; ++i) {
        augmented[i].resize(2 * n);
        augmented[i][n + i] = 1;
    }

    // Perform Gauss-Jordan elimination
    for (int i = 0; i < n; ++i) {
        // Find the pivot element
        double pivot = augmented[i][i];
        if (fabs(pivot) < 1e-10) {
            throw std::runtime_error("Matrix is singular and cannot be inverted.");
        }

        // Normalize the pivot row
        for (int j = 0; j < 2 * n; ++j) {
            augmented[i][j] /= pivot;
        }

        // Eliminate the pivot column in other rows
        for (int k = 0; k < n; ++k) {
            if (k != i) {
                double factor = augmented[k][i];
                for (int j = 0; j < 2 * n; ++j) {
                    augmented[k][j] -= factor * augmented[i][j];
                }
            }
        }
    }

    // Extract the inverse matrix from the augmented matrix
    std::vector<std::vector<double>> inverse(n, std::vector<double>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            inverse[i][j] = augmented[i][n + j];
        }
    }

    return inverse;
}

// Function to perform matrix-vector multiplication
std::vector<double> multiply(const std::vector<std::vector<double>>& matrix, const std::vector<double>& vector) {
    std::vector<double> result(vector.size(), 0);
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < vector.size(); ++j) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    return result;
}

std::vector <double> computePositions(vector <vector <double>> qMatrix, vector <double> dx) {
    // Compute the inverse of Q
    std::vector<std::vector<double>> Q_inv = inverse(qMatrix);

    // Multiply Q_inv by -dx
    for(auto & val : dx) {
        val = -val;
    }

    std::vector<double> x;
    x.resize(dx.size());
    x = multiply(Q_inv, dx);

    return x;
}

// int main() {
//     // Example input
//     std::vector<std::vector<double>> Q = {
//         {4, 7},
//         {2, 6}
//     };

//     std::vector<double> dx = {5, -3};

//     // Compute the inverse of Q
//     std::vector<std::vector<double>> Q_inv = inverse(Q);

//     // Multiply Q_inv by -dx
//     for(auto & val : dx) {
//         val = -val;
//     }

//     std::vector<double> x = multiply(Q_inv, dx);

//     // Output the result
//     std::cout << "Solution x: ";
//     for (const double& val : x) {
//         std::cout << val << " ";
//     }
//     std::cout << std::endl;

//     return 0;
// }
