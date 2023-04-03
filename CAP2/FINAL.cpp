#include <iostream>
#include <vector>
#include <random>
#include <immintrin.h>
#include <ctime>
#include <array>
#include <string>

using namespace std;
constexpr int fourxfour{ 4 };


template <int N>
void print_matrix(int matrix[N][N], int &dimension, string output_text) {
	cout << endl << output_text << endl;
	for (int i = 0;i < dimension;i++) {
		for (int j = 0; j < dimension; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}


/*
vector<vector<int>> multipx86(vector<vector<int>> m1,
                                vector<vector<int>> m2,
                                int dimension){
    vector<vector<int>> mResult(dimension, vector<int>(dimension));
    vector<int> aux1(dimension); 
    vector<int> aux2(dimension);
    for (int i = 0; i < dimension; i++) {
        aux1 = m1[i]; //Vector m1[i]
        for (int y = 0; y < dimension; y++) {
            for (int j = 0; j < dimension; j++) {
                aux2[j] = m2[j][y]; //Vector m2[j][i]
            }

            int sum = 0;

             __asm {
                inicializar:
                    mov eax, 0; //i = 0
                    mov ebx, 0; //sum
                    mov esi, [aux1]; //Base address of m1 row
                    mov edi, [aux2]; //Base address of m2 column

                for_loop:
                    cmp eax, 3; //if i >= dimension -> end_loop
                    jae end_loop;

                    mov esi, [esi + eax * 4]; //Next element of aux1
                    mov edi, [edi + eax * 4]; //Next element of aux2

                    mov ecx, esi;
                    imul ecx, edi; //Multiplication of both elements

                    add ebx, ecx; //Add it to the sum

                    inc eax; //increment of i
                    jmp for_loop;

                end_loop:
                   mov sum, ebx;
            }

            mResult[i][y] = sum;
        }   
    }
    return mResult;
}
*/

template <int N>
void cpp_multiplication(int m1[N][N], int m2[N][N], int dim) {
    int result[N][N]{ 0 };
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
}

template <int size>
void x86_multiplication(int (&m1)[size][size], int (&m2)[size][size], int N) {
	
	int product[size][size]{};

    for (int i{}; i < N; i++) {
        int* row1{ m1[i] };
        for (int j{}; j < N; j++) {
            int* col2{ m2[j] };
            int sum{};
            
            __asm {

                jmp start;
                start:
					mov esi, 0; // sum
					mov ecx, 0; // counter
                    jmp for_loop;


                for_loop:
                    cmp ecx, N;
                    jae end_loop;
					mov ebx, [row1];
					mov edx, [col2];
                    mov ebx, [ebx + ecx * 4];
                    mov edx, [edx + ecx * 4];

                    mov eax, ebx;
                    mul edx;
                    add esi, eax;

                    inc ecx;
                    jmp for_loop;

				end_loop:
                    mov [sum], esi;
            }
            product[i][j] = sum;
        }
    }
}

template <int N>
void sse_multiplication(int mat1[N][N], int mat2[N][N]) {
    int i, j, k; // initialize loop counters
    int result[N][N]{ 0 };
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

template <int size>
array<array<int,size>, size> generate_random_matrix(int dim) {

    array<array<int,size>, size> matrix;

    random_device rd; // obtain random number from hardware
    mt19937 gen(rd()); // seed the generator
    uniform_int_distribution<> distr(1, 501);

    for (int i{}; i < dim; i++) {
        for (int j{}; j < dim; j++) {
            matrix[i][j] = distr(gen);
        }
    }

    return matrix;

}

template <int matrix_size>
void runBenchmark(int iterations) {
    float time_c{};
    float time_x86{};
    float time_sse{};

	int m1[matrix_size][matrix_size]{};
	int m2[matrix_size][matrix_size]{};

	random_device rd; // obtain random number from hardware
	mt19937 gen(rd()); // seed the generator
	uniform_int_distribution<> distr(1, 501);

	for (int i{}; i < matrix_size; i++) {
		for (int j{}; j < matrix_size; j++) {
			m1[i][j] = distr(gen);
			m2[i][j] = distr(gen);
		}
	}

    for (int i{}; i < iterations; i++) {

        
        clock_t c_start = clock();
        cpp_multiplication<matrix_size>(m1, m2, matrix_size);
        clock_t c_end = clock();

        time_c += float(c_end - c_start) / CLOCKS_PER_SEC;

        clock_t x86_start = clock();
        x86_multiplication<matrix_size>(m1, m2, matrix_size);
        clock_t x86_end = clock();

        time_x86 += float(x86_end - x86_start) / CLOCKS_PER_SEC;
        
        clock_t sse_start = clock();
        sse_multiplication<matrix_size>(m1, m2);
        clock_t sse_end = clock();

        time_sse += float(sse_end - sse_start) / CLOCKS_PER_SEC;

    }

    cout << "Running: " << iterations << " iterations.\n";
    cout << "On " << matrix_size << "x" << matrix_size << " matrices.\n";
    cout << "Time in C++ code: " << time_c << '\n';
    cout << "Time in x86 code: " << time_x86 << '\n';
    cout << "Time in SSE code: " << time_sse << '\n';
    cout << endl;
}

int main() {
    //Ask for dimension of the arrays
    constexpr int dimension{ fourxfour };
    
    for (int i{1}; i <= 10; i++) {
        runBenchmark<4>(10000 * i);
    }
    for (int i{1}; i <= 10; i++) {
        runBenchmark<8>(10000 * i);
    }
    for (int i{1}; i <= 10; i++) {
        runBenchmark<12>(10000 * i);
    }
    for (int i{1}; i <= 10; i++) {
        runBenchmark<16>(10000 * i);
    }



    //Create with rand() both arrays

   /*
    //Print matrix 1 and 2
    cout << endl << "Esta es la primera matriz: " << endl;
    for (int i = 0;i < dimension;i++) {
        for (int j = 0; j < dimension; j++) {
            cout << m1[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl << "Esta es la segunda matriz: " << endl;
    for (int i = 0;i < dimension;i++) {
        for (int j = 0; j < dimension; j++) {
            cout << m2[i][j] << " ";
        }
        cout << endl;
    }

    //Set a clock for every call to the functions in order to see what time lasts each one

   // cpu time of  CPP

    auto start_cpp = chrono::high_resolution_clock::now();
    //print_matrix(toy_bench::cpp_multiplication(m1, m2, dimension), dimension, "m1*m2 in cpp");
    //toy_bench::cpp_multiplication(m1, m2, dimension);
    sse_multiplication<dimension>(m1, m2);
    cpp_multiplication<dimension>(m1, m2, dimension);
    auto end_cpp = chrono::high_resolution_clock::now();


    chrono::duration<double> time_taken_cpp = end_cpp - start_cpp;
    cout << "Time in s: " << time_taken_cpp.count() << endl;
	
    
     // cpu time of  x86
    auto start_x86 = chrono::high_resolution_clock::now();
    x86_multiplication<dimension>(m1,m2, dimension);
    auto end_x86 = chrono::high_resolution_clock::now();


    chrono::duration<double> time_taken_x86 = end_x86 - start_x86;
    cout << "Time in s: " << time_taken_x86.count() << endl;
	
    */
   
    return 0;
}
