#include "lightblas/math_functions.hpp"

void my_sgemm(const int M, const int N, const int K,
	const float* A, const float* B, float* C){
  // Add your own code here
  int i,j,k;
  for(i=0; i<M; i++)
  {   for(j=0; j<N; j++)
      {   for(k=0; k<K; k++)
              *(C+i*M+j) += *(A+i*M+k) * *(B+k*K+j);
	  }
  } 
}

void my_sconv(const int kernel_size, const int pad, const int stride,
		const int ic, const int ih, const int iw, const int oc,
		const float* weight, const float* input, float* output){
// Add your own code here
    int oc_0, h_0, w_0, ic_0, k_i, k_y;
    h = (ih+2*pad)/stride+1;
    w = (iw+2*pad)/stride+1;
	for(oc_0=0; oc_0<oc; oc_0++)
	{   for(h_0=0; h_0<h; h_0++)
	    {   for(w_0=0; w_0<w; h_0++)
	        {   for(ic_0=0; ic_0<ic; ic_0++)
	            {   for(k_i=0; k_i<k; k_i++)
	                {   for(k_y=0; k_y<k; k_y++)
	                    {   *(*(*(output+oc_0)+h_0)+w_0) += *(*(*(*(w+oc_0)+ic_0)+k_i)+k_y) * *(*(*(*(*(input+h_0)+w_0)+ic_0)+k_i)+k_y);
						}
					}
				}
	        }
		}
	} 
}
