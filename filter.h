#ifndef SAMPLEFILTER_H_
#define SAMPLEFILTER_H_

/*

FIR filter designed with
 http://t-filter.appspot.com

sampling frequency: 972500 Hz

* 0 Hz - 50000 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = 4.17389942395653 dB

* 60000 Hz - 486250 Hz
  gain = 0
  desired attenuation = -45 dB
  actual attenuation = -45.004121117208804 dB

*/

#define SAMPLEFILTER_TAP_NUM 47
//#define SAMPLEFILTER_TAP_NUM 115

typedef struct {
  float history[SAMPLEFILTER_TAP_NUM];
  unsigned int last_index;
} SampleFilter;

void SampleFilter_init(SampleFilter* f);
void SampleFilter_put(SampleFilter* f, float input);
float SampleFilter_get(SampleFilter* f);

#endif
