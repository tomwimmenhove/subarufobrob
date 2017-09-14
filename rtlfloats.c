#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include <rtl-sdr.h>

/* >>> Done

Generating: '/home/tom/radio/hack/subarufobrob/top_block.py'

Executing: /usr/bin/python -u /home/tom/radio/hack/subarufobrob/top_block.py

Using Volk machine: sse4_2_64_orc
gr-osmosdr 0.1.4 (0.1.4) gnuradio 3.7.9
built-in source types: file fcd rtl rtl_tcp rfspace redpitaya 
Using device #0 Realtek RTL2838UHIDIR SN: 00000013
Found Rafael Micro R820T tuner

Setting tuner gain mode to 1
Setting agc mode to 0

Setting sample rate to 1e+06
Exact sample rate is: 1000000.026491 Hz
setting center freq to 4.3392e+08
Setting tuner gain to 87

buf_num: 15, buf_len: 2621440

*/

typedef struct
{
	float i;
	float q;
} __attribute__((packed)) complexSample;

#define DEFAULT_BUF_LENGTH              (16 * 16384)

complexSample out[DEFAULT_BUF_LENGTH / 2];
complexSample lut[0x10000];;

void rtlsdr_callback(unsigned char *buf, uint32_t len, void *ctx)
{
	unsigned short *s = (unsigned short*) buf;

	int nout = len / 2;

	complexSample* outp = out;

	for (int i = 0; i < nout; ++i)
		      *outp++ = lut[ *(s + i) ];


	if (write(1, out, nout * sizeof(complexSample)) == -1)
	{
		perror("write():");
		exit(1);
	}
//	fprintf(stderr, "Bufferlen: %d\n", len);
}

int main(int argc, char **argv)
{
	rtlsdr_dev_t *dev = NULL;
	unsigned int dev_index = 0;
	int ret;
	uint32_t sampleRate = 1000000;
	double freq = 433.92e+6;
//	int tunerGain = 87;
	unsigned short **buffer;


	if (argc < 2)//3)
	{
		fprintf(stderr, "usage: %s <frequency> <tuner gain>\n", argv[0]);
//		fprintf(stderr, "tuner gain of 87 works for me\n");
		return -1;
	}

	freq = atof(argv[1]);
//	tunerGain = atoi(argv[2]);

	ret = rtlsdr_open(&dev, dev_index);
	if (ret < 0)
	{
		fprintf(stderr, "Can't open device");
		return -1;
	}

	rtlsdr_set_tuner_gain_mode(dev, 1);
	rtlsdr_set_agc_mode(dev, 0);
	rtlsdr_reset_buffer(dev);


	// autogtain test
	rtlsdr_set_tuner_gain_mode(dev, 0);
        rtlsdr_set_agc_mode(dev, 1);

	rtlsdr_set_sample_rate(dev, sampleRate);
	rtlsdr_set_center_freq(dev, (uint32_t) freq);
	//rtlsdr_set_freq_correction( _dev, (int)ppm );
	//rtlsdr_set_agc_mode(_dev, int(automatic));
//	rtlsdr_set_tuner_gain(dev, tunerGain); // works...-ish
	//rtlsdr_set_tuner_if_gain( _dev, stage, int(gains[ stage ] * 10.0));


	for (unsigned int i = 0; i < 0x10000; i++)
	{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		lut[i].i = (((float)(i & 0xff)) - 127.4f) * (1.0f/128.0f);
		lut[i].q = (((float)(i >> 8)) - 127.4f) * (1.0f/128.0f);
#else // BIG_ENDIAN
		lut[i].i = (((float)(i >> 8)) - 127.4f) * (1.0f/128.0f);
		lut[i].q = (((float)(i & 0xff) - 127.4f) * (1.0f/128.0f);
#endif
	}



	ret = rtlsdr_read_async(dev, rtlsdr_callback, (void *) NULL, 0, DEFAULT_BUF_LENGTH);


	if (ret == -1)
	{
		fprintf(stderr, "rtlsdr_read_async returned with %d\n", ret);
		return -1;
	}



	return 0;
}

