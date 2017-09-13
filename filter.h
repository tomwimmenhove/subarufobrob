// Designed using http://t-filter.engineerjs.com/
#ifndef SAMPLEFILTER_H_
#define SAMPLEFILTER_H_

/*

   FIR filter designed with
http://t-filter.appspot.com

sampling frequency: 1000000 Hz

 * 0 Hz - 50000 Hz
 gain = 1
 desired ripple = 5 dB
 actual ripple = 0 dB

 */

#define SAMPLEFILTER_TAP_NUM 5

typedef struct {
	double history[SAMPLEFILTER_TAP_NUM];
	unsigned int last_index;
} SampleFilter;

void SampleFilter_init(SampleFilter* f);
void SampleFilter_put(SampleFilter* f, double input);
double SampleFilter_get(SampleFilter* f);

#endif
