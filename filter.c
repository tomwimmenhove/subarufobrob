// Designed using http://t-filter.engineerjs.com/
#include "filter.h"

static double filter_taps[SAMPLEFILTER_TAP_NUM] = {
	-0.0034473462146292415,
	-0.005000451673234299,
	-0.007423581552162021,
	-0.009275547438522096,
	-0.009578136121711258,
	-0.007236543607181547,
	-0.0012434176693360227,
	0.009084549372884047,
	0.02388722821461575,
	0.0426143538369512,
	0.06399745041435487,
	0.08615195572896084,
	0.10680720614706975,
	0.12364241765392411,
	0.13465638205597324,
	0.13848416526155305,
	0.13465638205597324,
	0.12364241765392411,
	0.10680720614706975,
	0.08615195572896084,
	0.06399745041435487,
	0.0426143538369512,
	0.02388722821461575,
	0.009084549372884047,
	-0.0012434176693360227,
	-0.007236543607181547,
	-0.009578136121711258,
	-0.009275547438522096,
	-0.007423581552162021,
	-0.005000451673234299,
	-0.0034473462146292415
};

void SampleFilter_init(SampleFilter* f) {
	int i;
	for(i = 0; i < SAMPLEFILTER_TAP_NUM; ++i)
		f->history[i] = 0;
	f->last_index = 0;
}

void SampleFilter_put(SampleFilter* f, double input) {
	f->history[f->last_index++] = input;
	if(f->last_index == SAMPLEFILTER_TAP_NUM)
		f->last_index = 0;
}

double SampleFilter_get(SampleFilter* f) {
	double acc = 0;
	int index = f->last_index, i;
	for(i = 0; i < SAMPLEFILTER_TAP_NUM; ++i) {
		index = index != 0 ? index-1 : SAMPLEFILTER_TAP_NUM-1;
		acc += f->history[index] * filter_taps[i];
	};
	return acc;
}

