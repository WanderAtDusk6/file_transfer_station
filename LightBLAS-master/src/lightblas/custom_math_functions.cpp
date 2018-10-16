#include "lightblas/math_functions.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
void my_sgemm(const int M, const int N, const int K,
	const float* A, const float* B, float* C){
  // Add your own code here
}

void my_sconv(const int kernel_size, const int pad, const int stride,
		const int ic, const int ih, const int iw, const int oc,
		const float* weight, const float* input, float* output){
   // Add your own code here

 }

*/

inline bool is_a_ge_zero_and_a_lt_b(int a, int b) {
	return static_cast<unsigned>(a) < static_cast<unsigned>(b);
}

void my_sgemm(const int M, const int N, const int K, const float* A, const float* B, float* C) {
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < N; ++j) {
			C[i*N + j] = 0;   // ��ʾC��ÿһ��Ԫ�� 
			for (int z = 0; z < K; ++z) {  // z�ȱ�ʾA���У��ֱ�ʾB����  
				C[i*N + j] += A[i*K + z] * B[z*N + j];
			}
		}
	}
}

void img2col(const int kernel_size, const int pad, const int stride, const int ic, const int ih, const int iw, const float* input, float* img_col) {

	//�����������ͼ��ĸߺͿ�
	const int oh = (ih - kernel_size + 2 * pad) / stride + 1;
	const int ow = (iw - kernel_size + 2 * pad) / stride + 1;
	//const int ohw = oh * ow; // ����ͼ��ת����������ֵ


	//�����������뵥ͨ��ͼ�����������
	const int cc_size = ih * iw;
	//�������˵�ͨ������������
	//const int kk_size = kernel_size * kernel_size;

	//��һ��forѭ����ʾ����ľ���ͨ�����;��������ͼ��ͨ����һ���ģ�ÿ�δ���һ������ͨ������Ϣ
	for (int ic_ = ic; ic_--; input += cc_size) {  //???? ic_ dֵû�������𣿣�����
		//�ڶ����͵�����forѭ����ʾ�������ͨ�������ĳһ�У�ͬʱ�����������ͨ�����������
		for (int kernel_h = 0; kernel_h < kernel_size; kernel_h++) {
			for (int kernel_w = 0; kernel_w < kernel_size; kernel_w++) {
				int ih_ = -pad + kernel_h;  //�������ҵ�������е�ĳһ��������ͼ���еĵ�һ�����������������
				//���ĸ��͵����forѭ����ʾ�������ͨ�������ĳһ�У�ͬʱ�����������ͨ�����������
				for (int oh_ = oh; oh_; oh_--) {
					if (!is_a_ge_zero_and_a_lt_b(ih_, ih)) {//�������õ�������ͼ�����ֵ����С������ߴ�������ͼ��ĸ�(����Ϊpad)
						for (int ow_ = ow; ow_; ow_--) {
							*(img_col++) = 0; //��ô������������ľ����ϵ�λ����Ϊ0
						}
					}
					else {
						int iw_ = -pad + kernel_w; //�������ҵ�������е�ĳһ��������ͼ���еĵ�һ�����������������
						for (int ow_ = oh; ow_; ow_--) {
							if (is_a_ge_zero_and_a_lt_b(iw_, iw)) {//�������õ�������ͼ�����ֵ�������ڵ����������С������ͼ��Ŀ�(���в���pad)
								*(img_col++) = input[ih_ * iw + iw_];//����������ͼ�϶�Ӧ������ŵ����������
							}
							else {
								//���򣬼���õ�������ͼ�����ֵ����С������ߴ�������ͼ��Ŀ�(����Ϊpad)
								*(img_col++) = 0;//�����и�������������ϵ�λ����Ϊ0
							}
							iw_ += stride;//���տ��򲽳�����������Ϲ̶���������ͼ���ϻ�������������
						}
					}
					ih_ += stride;//���ո߷��򲽳�����������Ϲ̶���������ͼ���ϻ�������������
				}
			}
		}
	}
}


void my_sconv(const int kernel_size, const int pad, const int stride, const int ic, const int ih, const int iw, const int oc, const float* weight, const float* input, float* output) {
	const int k = ic * kernel_size * kernel_size; //����˾������ֵ��Ҳ������ͼ��ת����������ֵ������˵��и�Ϊ��oc

	//�����������ͼ��ĸߺͿ�
	const int oh = (ih - kernel_size + 2 * pad) / stride + 1;
	const int ow = (iw - kernel_size + 2 * pad) / stride + 1;
	const int ohw = oh * ow; // ����ͼ��ת����������ֵ

	//�������˵��������� ��Ϊk
	//const int kk_size = kernel_size * kernel_size * ic;

	//float input_matrix[1000] = { 0 };
	float *input_matrix = new float[k*ohw]();
	img2col(kernel_size, pad, stride, ic, ih, iw, input, input_matrix);
	my_sgemm(oc, ohw, k, weight, input_matrix, output);
	delete []input_matrix;

	//weight += oc * k;
	//*input_matrix += k * ohw;
	//*res += kc;

}
