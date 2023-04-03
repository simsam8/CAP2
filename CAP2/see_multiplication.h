#ifndef SSE_MULTIPLICATION_H
#define SSE_MULTIPLICATION_H
#include "immintrin.h"
namespace toy_bench {
	template <int N>
	int sse_multiplication(int mat1[N][N], int mat2[N][N]);
}
#endif
