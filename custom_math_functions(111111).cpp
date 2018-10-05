#include "lightblas/math_functions.hpp"

void my_sgemm(const int M, const int N, const int K,
	const float* A, const float* B, float* C){
  // Add your own code here
  int i,j,k;
  for(i=0; i<M; i++)
  {   for(j=0; j<N; j++)
      {   for(k=0; k<K; k++)
              *(C+i*M+j) = *(A+i*M+k) + *(B+k*K+j);
	  }
  } 
}

void my_sconv(const int kernel_size, const int pad, const int stride,
		const int ic, const int ih, const int iw, const int oc,
		const float* weight, const float* input, float* output){
// Add your own code here
}
