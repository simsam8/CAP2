#include <iostream>
#include <vector>
#include <chrono>
#include "immintrin.h"

using namespace std;

const int N =12 ;

void matmulSSE_v2(int mat1[N][N], int mat2[N][N], int result[N][N]) {
    int i, j, k; // initialize loop counters

    __m128i vA, vB, vR; // declare SSE registers for operands and result

    for (i = 0; i < N; ++i) { // iterate over rows of first matrix
        for (k = 0; k < N; ++k) { // iterate over columns of first matrix and rows of second matrix
            vA = _mm_set1_epi32(mat1[i][k]); // load ith row of first matrix into SSE register
            for (j = 0; j < N; j += 4) { // iterate over columns of second matrix, unroll loop by 4
                vB = _mm_loadu_si128((__m128i*) & mat2[k][j]); // load 4 integers from j 'th column of second matrix into SSE register
                vR = _mm_loadu_si128((__m128i*) & result[i][j]); // load 4 integers from j 'th column of output matrix into SSE register
                vR = _mm_add_epi32(vR, _mm_mullo_epi32(vA, vB)); // perform element-wise multiplication and addition of operands
                _mm_storeu_si128((__m128i*) & result[i][j], vR); // store 4 integers of result back to memory
            }
        }
    }
}

/*
int main() {
    // Definir matrices de ejemplo
    int matrix_a[N][N];
    int matrix_b[N][N];
    int contador = 1;
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < N;j++) {
            matrix_a[i][j] = contador;
            contador++;
        }
    }
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < N;j++) {
            matrix_b[i][j] = contador;
            contador++;
        }
    }

    int result[N][N] = { 0 };

    // Medir tiempo de ejecución
    auto start = chrono::high_resolution_clock::now();

    // Multiplicar matrices
    matmulSSE_v2(matrix_a, matrix_b, result);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Tiempo de ejecución: " << duration.count() << " segundos" << endl;

    // Imprimir matriz resultado
    cout << "Matriz resultado:" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << result[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
*/
