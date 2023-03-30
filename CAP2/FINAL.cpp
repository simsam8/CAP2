#include <iostream>
#include <vector>
#include "cpp_multiplication.h"
#include <chrono>
#include <string>

using namespace std;


// function to print out matrices
void print_matrix(vector<vector<int>> matrix, int dimension, string output_text) {
	cout << endl << output_text << endl;
	for (int i = 0;i < dimension;i++) {
		for (int j = 0; j < dimension; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}


vector<vector<int>> multipx86(vector<vector<int>> m1, vector<vector<int>> m2, int dimension){
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



int main() {

    //Ask for dimension of the arrays
    int dimension = 0;

    cout << "Introduce la dimension de la matriz: " << endl;
    cin >> dimension;

    //Declare the matrix 1 and 2
    vector<vector<int>> m1(dimension, vector<int>(dimension));
    vector<vector<int>> m2(dimension, vector<int>(dimension));


    //Create with rand() both arrays
    for (int i = 0;i < dimension;i++) {
        for (int j = 0; j < dimension; j++) {
            m1[i][j] = rand() % 101;
            m2[i][j] = rand() % 101;
        }
    }

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
    */

    //Set a clock for every call to the functions in order to see what time lasts each one

    //Call divisioncpp(), divisionx86(), divisionSSE()

    print_matrix(m1, dimension, "Esta es la primera matriz: ");
    print_matrix(m2, dimension, "Esta es la segunda matriz: ");

   // cpu time of  CPP
    auto start_cpp = chrono::high_resolution_clock::now();
    print_matrix(cpp_mul::cpp_multiplication(m1, m2, dimension), dimension, "m1*m2 in cpp");
    auto end_cpp = chrono::high_resolution_clock::now();


    chrono::duration<double> time_taken_cpp = end_cpp - start_cpp;
    cout << "Time in s: " << time_taken_cpp.count() << endl;
	
    
     // cpu time of  x86
    auto start_x86 = chrono::high_resolution_clock::now();
    print_matrix(multipx86(m1, m2, dimension), dimension, "m1*m2 in x86");
    auto end_x86 = chrono::high_resolution_clock::now();


    chrono::duration<double> time_taken_x86 = end_x86 - start_x86;
    cout << "Time in s: " << time_taken_x86.count() << endl;
	
	
   //Show the results of each function
   
    return 0;
}

/*
divisioncpp(){
 }
*/  

/*
divisionSSE(){
     
}
*/
