// Designed using http://t-filter.engineerjs.com/
#include "filter.h"

static double filter_taps[SAMPLEFILTER_TAP_NUM] = {
	-4.4408920985006264e-17,
	4.4408920985006264e-17,
	1,
	4.4408920985006264e-17,
	-4.4408920985006264e-17
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

