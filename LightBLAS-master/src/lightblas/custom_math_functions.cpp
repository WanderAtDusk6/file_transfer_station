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
			C[i*N + j] = 0;   // 表示C的每一个元素 
			for (int z = 0; z < K; ++z) {  // z既表示A的列，又表示B的行  
				C[i*N + j] += A[i*K + z] * B[z*N + j];
			}
		}
	}
}

void img2col(const int kernel_size, const int pad, const int stride, const int ic, const int ih, const int iw, const float* input, float* img_col) {

	//计算卷积层输出图像的高和宽
	const int oh = (ih - kernel_size + 2 * pad) / stride + 1;
	const int ow = (iw - kernel_size + 2 * pad) / stride + 1;
	//const int ohw = oh * ow; // 输入图像转换后矩阵的列值


	//计算卷积层输入单通道图像的数据容量
	const int cc_size = ih * iw;
	//计算卷积核单通道的数据容量
	//const int kk_size = kernel_size * kernel_size;

	//第一个for循环表示输出的矩阵通道数和卷积层输入图像通道是一样的，每次处理一个输入通道的信息
	for (int ic_ = ic; ic_--; input += cc_size) {  //???? ic_ d值没有限制吗？？？？
		//第二个和第三个for循环表示了输出单通道矩阵的某一列，同时体现了输出单通道矩阵的行数
		for (int kernel_h = 0; kernel_h < kernel_size; kernel_h++) {
			for (int kernel_w = 0; kernel_w < kernel_size; kernel_w++) {
				int ih_ = -pad + kernel_h;  //在这里找到卷积核中的某一行在输入图像中的第一个操作区域的行索引
				//第四个和第五个for循环表示了输出单通道矩阵的某一行，同时体现了输出单通道矩阵的列数
				for (int oh_ = oh; oh_; oh_--) {
					if (!is_a_ge_zero_and_a_lt_b(ih_, ih)) {//如果计算得到的输入图像的行值索引小于零或者大于输入图像的高(该行为pad)
						for (int ow_ = ow; ow_; ow_--) {
							*(img_col++) = 0; //那么将该行在输出的矩阵上的位置置为0
						}
					}
					else {
						int iw_ = -pad + kernel_w; //在这里找到卷积核中的某一列在输入图像中的第一个操作区域的列索引
						for (int ow_ = oh; ow_; ow_--) {
							if (is_a_ge_zero_and_a_lt_b(iw_, iw)) {//如果计算得到的输入图像的列值索引大于等于于零或者小于输入图像的宽(该列不是pad)
								*(img_col++) = input[ih_ * iw + iw_];//将输入特征图上对应的区域放到输出矩阵上
							}
							else {
								//否则，计算得到的输入图像的列值索引小于零或者大于输入图像的宽(该列为pad)
								*(img_col++) = 0;//将该行该列在输出矩阵上的位置置为0
							}
							iw_ += stride;//按照宽方向步长遍历卷积核上固定列在输入图像上滑动操作的区域
						}
					}
					ih_ += stride;//按照高方向步长遍历卷积核上固定行在输入图像上滑动操作的区域
				}
			}
		}
	}
}


void my_sconv(const int kernel_size, const int pad, const int stride, const int ic, const int ih, const int iw, const int oc, const float* weight, const float* input, float* output) {
	const int k = ic * kernel_size * kernel_size; //卷积核矩阵的列值，也是输入图像转换后矩阵的行值；卷积核的行高为：oc

	//计算卷积层输出图像的高和宽
	const int oh = (ih - kernel_size + 2 * pad) / stride + 1;
	const int ow = (iw - kernel_size + 2 * pad) / stride + 1;
	const int ohw = oh * ow; // 输入图像转换后矩阵的列值

	//计算卷积核的数据容量 即为k
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
