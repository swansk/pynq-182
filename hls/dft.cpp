#include <math.h> //Required for cos and sin functions
#include <hls_stream.h>
#include <ap_axi_sdata.h>

#define N 64 // DFT Size
typedef ap_axis<64,1,1,1> AXI_VAL;

typedef int data_t;

void dft(AXI_VAL* real_in, AXI_VAL* imag_in, AXI_VAL* real_out, AXI_VAL* imag_out) {
#pragma HLS INTERFACE axis register both port=real_in
#pragma HLS INTERFACE axis register both port=imag_in
#pragma HLS INTERFACE axis register both port=real_out
#pragma HLS INTERFACE axis register both port=imag_out
#pragma HLS INTERFACE ap_ctrl_none port=return
	int i, j;
	data_t w;
	data_t c, s;
	// Temporary arrays to hold the intermediate frequency domain results
	data_t temp_real[N];
	data_t temp_imag[N];
	// Calculate each frequency domain sample iteratively
	for (i = 0; i < N; i += 1) {
		temp_real[i] = 0;
		temp_imag[i] = 0;
		w = (2.0 * 3.141592653589 / N) * (double)i;
		// Calculate the jth frequency sample sequentially
		for (j = 0; j < N; j += 1) {
			// Utilize HLS tool to calculate sine and cosine values
			c = cos(j * w);
			s = sin(j * w);
			// Multiply the current phasor with the appropriate input sample and keep
			// running sum
			temp_real[i] += (real_in[j] * c - imag_in[j] * s);
			temp_imag[i] += (real_in[j] * s + imag_in[j] * c);
		}
	}
	// Perform an inplace DFT, i.e., copy result into the input arrays
	for (i = 0; i < N; i += 1) {
		real_out[i] = temp_real[i];
		imag_out[i] = temp_imag[i];
	}
}
