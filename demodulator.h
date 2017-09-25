#ifndef DEMODULATOR_H
#define DEMODULATOR_H

#include <ctype.h>

#include "runningavg.h"

typedef struct 
{
	void (*packetHandler)(unsigned char* packet);

	/* Bit demod */
	int state;
	int preambleGood;
	int lastDemodBit;
	int ending;
	int minPreamble;
	int manchPos;
	int bitpos;
	uint8_t byte;
	int bytePos;
	uint8_t packet[10];
	int retry;

	/* Sample demod */
	int lastBit;
	int sampleAt;
	int sampleNum;
	int samplesPerBit;
	runningAvgContext midPointCtx;
	runningAvgContext bitAvgCtx;
} DemodContext;

void demodBit(DemodContext* demodCtx, int bit);
void demodSample(DemodContext* demodCtx, double magnitude);
void demodInit(DemodContext* demodCtx, int samplesPerBit, int minPreambleBits, void (*packetHandler)(unsigned char* packet));

#endif // DEMODULATOR_H

