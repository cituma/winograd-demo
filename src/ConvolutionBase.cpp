#include <iostream>

extern const int kernel_size;

void ConvolutionBase(float* input, float* kernel, float* output, 
	int input_num, int output_num, int width, int height) {
	//input: nchw(1, input_num, height+2, width+2)
	//kernel: nchw(output_num, input_num, height, width)
	//output: nchw(1, output_num, height, width)

	int out_stride = width * height;
	int in_stride = (width + 2) * (height + 2);
	int kernel_stride = kernel_size + kernel_size;
	for (int n = 0; n < output_num; ++n) {

		float* kernel_ptr = &kernel[n * input_num * kernel_size * kernel_size];
		float* output_ptr = &output[n * out_stride];

		for (int h = 0; h < height; ++h) {
			for (int w = 0; w < width; ++w) {

				for (int c = 0; c < input_num; ++c) {
					for (int kh = 0; kh < kernel_size; ++kh) {
						for (int kw = 0; kw < kernel_size; ++kw) {
							int kernel_pos = c * kernel_stride + kh * kernel_size + kw;
							int input_pos = c * in_stride + (h + kh) * (width + 2) + (w + kw);
							output_ptr[h * width + w] += kernel_ptr[kernel_pos] * input[input_pos];
						}
					}
				}
			}
		}
	}
}
