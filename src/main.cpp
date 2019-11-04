#include <iostream>
#include <random>
#include <time.h>
#include "timer.h"

extern const int kernel_size = 3;

static void random_vector(std::vector<float>& matrix) {
	int size = static_cast<int>(matrix.size());
	unsigned int min = 0;
	unsigned int max = 255;
	clock_t start = clock();
	for (int i = 0; i < size; ++i) {
		unsigned int seed = static_cast<unsigned int>(i + start);
		static std::default_random_engine e(seed);
		static std::uniform_real_distribution<double> u(min, max);
		//matrix[i] = static_cast<float>(static_cast<int>(u(e)));
		matrix[i] = 1.f;
	}
}

static void clear_vector(std::vector<float>& v, float num = 0.f) {
	for (auto& f : v) {
		f = num;
	}
}

static void check_result(std::vector<float>& l, std::vector<float>& r) {
	for (int i = 0; i < l.size(); ++i) {
		if (l[i] != r[i]) {
			std::cout << "compare error in idx:" << i << "." << l[i] << ":" << r[i] << std::endl;
			if (i > 100) {
				return;
			}
		}
	}
}

extern void ConvolutionBase(float* input, float* kernel, float* output,
	int input_num, int output_num, int width, int height);

int main(int argc, char* argv[]) {
	int size = 100;
	size = (size / 4) * 4;
	int width = size;
	int height = size;
	int input_num = 8;
	int output_num = 16;
	// stride = 1; pad = 0;
	std::vector<float> input(input_num * (width+2) * (height+2));
	std::vector<float> kernel(output_num * input_num * kernel_size * kernel_size);	//k*n
	std::vector<float> output(output_num * width * height);
	std::vector<float> tmp_output(output_num * width * height);
	random_vector(input);
	random_vector(kernel);

	for (int n = 0; n < input_num; ++n) {
		int w = width + 2;
		int h = height + 2;
		int pos = n * h * w;
		for (int j = 0; j < h; ++j) {
			for (int i = 0; i < w; ++i) {
				int p = pos + j * w + i;
				if (i == 0 || i == (w - 1) || j == 0 || j == (h - 1)) {
					input[p] = 0.f;
				}
			}
		}
	}

	//unsigned long long cal_num = 2 * (unsigned long long)m * (unsigned long long)n * (unsigned long long)k;
	//double gflops = (double)cal_num / (double)(1024 * 1024 * 1024);

	HighClock clk;
	double cal_time = 0.;
#if 1
	clear_vector(output);
	// matrix multipl1 test
	clk.Start();
	ConvolutionBase(&input[0], &kernel[0], &output[0], input_num, output_num, width, height);
	clk.Stop();
	cal_time = clk.GetTime() / 1000000.; //s
	std::cout << "ConvolutionBase time: " << cal_time * 1000. << "ms." << std::endl;
	//std::cout << "ConvolutionBase time: " << cal_time * 1000. << "ms. GFLOPS/sec: " << gflops / cal_time << std::endl;
#endif

#if 0
	for (int c = 0; c < output_num; ++c) {
		for (int h = 0; h < height; ++h) {
			for (int w = 0; w < width; ++w) {
				std::cout << output[c * width * height + h * width + w] << " ";
			}
			std::cout << std::endl;
		}
	}
#endif
	tmp_output = output;

	check_result(tmp_output, output);

	return 0;
}