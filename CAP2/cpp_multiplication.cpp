
namespace toy_bench {

    template <int N>
    int cpp_multiplication(int m1[N][N], int m2[N][N], int result[N][N], int dim) {
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
