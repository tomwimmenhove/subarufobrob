#include "filter.h"

static float filter_taps[SAMPLEFILTER_TAP_NUM] = {
	-0.000325463741319,
	0.000756483757868,
	0.00146557833068,
	0.00095358339604,
	-0.00103854783811,
	-0.00319028855301,
	-0.00293344305828,
	0.00109442975372,
	0.00640950677916,
	0.00738019868731,
	0.000352530216333,
	-0.010853914544,
	-0.0155918952078,
	-0.00530383270234,
	0.01583962515,
	0.0297110620886,
	0.01755319722,
	-0.0204241275787,
	-0.056505702436,
	-0.0497136823833,
	0.023651458323,
	0.145785108209,
	0.260909825563,
	0.308036655188,
	0.260909825563,
	0.145785108209,
	0.023651458323,
	-0.0497136823833,
	-0.056505702436,
	-0.0204241275787,
	0.01755319722,
	0.0297110620886,
	0.01583962515,
	-0.00530383270234,
	-0.0155918952078,
	-0.010853914544,
	0.000352530216333,
	0.00738019868731,
	0.00640950677916,
	0.00109442975372,
	-0.00293344305828,
	-0.00319028855301,
	-0.00103854783811,
	0.00095358339604,
	0.00146557833068,
	0.000756483757868,
	-0.000325463741319,
};

#pragma GCC push_options
#pragma GCC optimize ("unroll-loops")

void SampleFilter_init(SampleFilter* f) {
	int i;
	for(i = 0; i < SAMPLEFILTER_TAP_NUM; ++i)
		f->history[i] = 0;
	f->last_index = 0;
}

void SampleFilter_put(SampleFilter* f, float input) {
	f->history[f->last_index++] = input;
	if(f->last_index == SAMPLEFILTER_TAP_NUM)
		f->last_index = 0;
}

float SampleFilter_get(SampleFilter* f) {
	float acc = 0;
	int index = f->last_index, i;
	for(i = 0; i < SAMPLEFILTER_TAP_NUM; ++i) {
		index = index != 0 ? index-1 : SAMPLEFILTER_TAP_NUM-1;
		acc += f->history[index] * filter_taps[i];
	};
	return acc;
}

#pragma GCC pop_options

