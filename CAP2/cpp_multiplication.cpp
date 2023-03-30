#include <iostream>
#include <vector>

using namespace std;

namespace cpp_mul {

    vector<vector<int>> cpp_multiplication(vector<vector<int>> m1, vector<vector<int>> m2, int dim) {
        vector<vector<int>> result(dim, vector<int>(dim));
        // Loop through rows of m1
        for (int i{}; i < dim; i++) {
            // Loop through columns of m2
            for (int j{}; j < dim; j++) {
                int sum{}; 
                // Loop through corresponding index in row of m1 and column of m2
                for (int k{}; k < dim; k++) {
                    sum += m1[i][k] * m2[k][j]; // multply elements
                }
                result[i][j] = sum; // dot product of [i][j]
            }
        }
        return result;
    }
}
