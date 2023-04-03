#ifndef X86_MULTIPLICATION_H
#define X86_MULTIPLICATION_H
namespace toy_bench {
	template <int size>
	int x86_multiplication(int(&m1)[size][size], int(&m2)[size][size], int N);
}
#endif
