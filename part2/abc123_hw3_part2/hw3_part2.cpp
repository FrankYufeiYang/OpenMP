//author Yufei Yang
#include <omp.h>

// complex algorithm for evaluation

void matrix_mult(double *A, double *B, double *C, int N)
{	
		
	//change this for different algorithms
	#define everything  
	



	#ifdef original
	
   for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			C[i * N + j] = 0;
		
			for (int k = 0; k < N; k++)
			{
				C[i * N + j] += A[i * N + k] * B[k * N + j];
			}
		}
	return;
	#endif

		//number of threads
	int P = 10;
	omp_set_num_threads(P);


	#ifdef orithread
	#pragma parallel 
	{
	
	#pragma omp for collapse(2)
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			C[i * N + j] = 0;
		
			for (int k = 0; k < N; k++)
			{
				C[i * N + j] += A[i * N + k] * B[k * N + j];
			}
		}
	}
	return;
	#endif

	//reference Tuning the maxtrix multiply algorithm @ sg532@cornell.edu
	#ifdef everything
	//loop reording with loop unrolling 
	double a1,a2,a3,a4,a5,a6,a7,a8;
	int k,j;
	for (int i = 0; i < N; ++i) {
		
		for ( k = 0; k  < (N-7) ; k+=8) {
			
			a1 = A[i*N+k];
			a2 = A[i*N+k+1];
			a3 = A[i*N+k+2];
			a4 = A[i*N+k+3];
			a5 = A[i*N+k+4];
			a6 = A[i*N+k+5];
			a7 = A[i*N+k+6];
			a8 = A[i*N+k+7];
			for ( j = 0; j < N; j++) {
				C[i*N + j] += a1 * B[k*N + j];
				C[i*N + j] += a2 * B[(k+1)*N + j];
				C[i*N + j] += a3 * B[(k+2)*N + j];
				C[i*N + j] += a4 * B[(k+3)*N + j];
				C[i*N + j] += a5 * B[(k+4)*N + j];
				C[i*N + j] += a6 * B[(k+5)*N + j];
				C[i*N + j] += a7 * B[(k+6)*N + j];
				C[i*N + j] += a8 * B[(k+7)*N + j];
			}
		
		}
		if(N % 8) {
			do {
				a1 = A[i*N + k];
				for (int j = 0; j < N; j++) {
					C[i*N + j] += B[k*N + j] * a1;
				}
			}while(++k < N);
		}

	}
	#endif

	#ifdef everythingthread
	//loop reording with copy 8 values from A
	
	double a1,a2,a3,a4,a5,a6,a7,a8;

	#pragma parallel
	{
	#pragma omp for
	for (int j = 0; j < N; ++j) {
		int k;
		for ( int k = 0; k  < (N-7) ; k+=8) {
			a1 = A[j*N+k];
			a2 = A[j*N+k+1];
			a3 = A[j*N+k+2];
			a4 = A[j*N+k+3];
			a5 = A[j*N+k+4];
			a6 = A[j*N+k+5];
			a7 = A[j*N+k+6];
			a8 = A[j*N+k+7];
			
			for (int i = 0; i < N; ++i) {
				C[j*N + i] += B[k*N + i] * a1;
				C[j*N + i] += B[(k+1)*N + i] * a2;
				C[j*N + i] += B[(k+2)*N + i] * a3;
				C[j*N + i] += B[(k+3)*N + i] * a4;
				C[j*N + i] += B[(k+4)*N + i] * a5;
				C[j*N + i] += B[(k+5)*N + i] * a6;
				C[j*N + i] += B[(k+6)*N + i] * a7;
				C[j*N + i] += B[(k+7)*N + i] * a8;
			}
		
		}
		if(N % 8 && omp_get_thread_num==0) {
			do {
				a1 = A[j*N + k];
				for (int i = 0; i < N; ++i) {
					C[j*N + i] += B[k*N + i] * a1;
				}
			}while(++k < N);
		}
		

	}
	}
	#endif
}

