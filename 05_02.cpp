/*
Исследовать эффективность распараллеливания вычислений на основе многопоточности операции комплекснозначных 
матриц общего вида (достаточно большой размерности) при использовании стандартных функций BLAS.
*/

// Matr_Mult_BLAS.cpp 
#include <time.h> 
#include <iostream> 
double* APtr = NULL, * BPtr = NULL, * CPtr = NULL;
#define A(i,j) APtr[N*((j)-1)+(i)-1] 
#define B(i,j) BPtr[N*((j)-1)+(i)-1] 
#ifdef _WIN32 
#define DGEMM_Func DGEMM 
#else 
#define DGEMM_Func dgemm_ 
#endif 
extern "C" void DGEMM_Func(char* transa, char* transb,
	int* m, int* n, int* k,
	double* alpha, double* a, int* lda,
	double* b, const int* ldb,
	double* beta, double* c, int* ldc);
using namespace std;
void Tst_Func(int N)
{
	APtr = new double[N * N];
	BPtr = new double[N * N];
	CPtr = new double[N * N];
	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= N; j++)
		{
			A(i, j) = i * j; // Инициализация матриц 
			B(i, j) = i * j;
		}
	double alpha = 1, beta = 0;
	char transa = 'N', transb = 'N';
	double Tms = clock();
	DGEMM_Func(&transa, &transb, &N, &N, &N,
		&alpha, APtr, &N, BPtr, &N,
		&beta, CPtr, &N); //Перемножение матриц 
	Tms = (clock() - Tms) / CLOCKS_PER_SEC;
	cout << "Time=" << Tms << " sec" << endl;
	delete[] CPtr;
	delete[] BPtr;
	delete[] APtr;
}
int main(void)
{
	for (int N = 1024; N <= 4098; N *= 2)
		Tst_Func(N);
}

/*

*/
