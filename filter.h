#ifndef SAMPLEFILTER_H_
#define SAMPLEFILTER_H_

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 1000000 Hz

* 0 Hz - 50000 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = 4.137265934511618 dB

* 100000 Hz - 500000 Hz
  gain = 0
  desired attenuation = -50 dB
  actual attenuation = -50.07784704888356 dB

*/

#define SAMPLEFILTER_TAP_NUM 31

typedef struct {
  double history[SAMPLEFILTER_TAP_NUM];
  unsigned int last_index;
} SampleFilter;

void SampleFilter_init(SampleFilter* f);
void SampleFilter_put(SampleFilter* f, double input);
double SampleFilter_get(SampleFilter* f);

#endif
