using namespace std;

namespace toy_bench {
	template <int size>
	int x86_multiplication(int (&m1)[size][size], int (&m2)[size][size], int N) {
		
		int product[size][size]{};

		__asm {
			jmp start;
		start:
			mov edx, [m1]; // Base adress of m1
			mov ebx, [m2]; // Base adress of m2
			mov edi, [product]; // Base adress of product matrix
			mov ecx, 0; // iterator
			jmp loop_row_m1;

		loop_row_m1:
			cmp ecx, N; // if i >= dimension
			jae end_mul; // exit and return

			mov edx, [edx + ecx * 4]; // next row of m1
			mov edi, [edi + edi * 4]; // next row of product

			inc ecx;
			push ecx;
			mov ecx, 0;
			jmp loop_col_m2;

			loop_col_m2:
				//mov edi, 0; // iterator
				cmp ecx, N;
				//cmp edi, N; // if j >= dimension
				jae loop_row_m1; // jump back to row loop

				mov ebx, [ebx + ecx * 4]; // next column of m2
				mov edi, [edi + ecx * 4]; // next element in row of product
				mov eax, 0; // initialize sum for multiplication
				inc ecx;
				push ecx;
				mov ecx, 0;
				jmp loop_multiply;

				loop_multiply:
					cmp ecx, N; // if k >= dimension
					jae loop_col_m2; // jump back to col loop

					mov ebp, [edx + ecx * 4]; // element in m1 row
					mov esi, [ebx + ecx * 4]; // element in m2 column

					imul ebp, esi; // mulitply both elements

					add eax, ebp; // add to sum
					mov esi, eax; // move sum to position in product

					inc esp;

			pop ecx; // restore value for col_m2 loop
			loop loop_col_m2;

		pop ecx; // restore value for row_m1 loop

		loop loop_row_m1;

					

		end_mul:

		}
		return product;
	}

}