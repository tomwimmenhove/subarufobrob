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
	int maxPreambleTimingError;
	int lastDemodBit;
	uint64_t lastOneSampledAt;
	int ending;
	int minPreamble;
	int manchPos;
	int bitpos;
	uint8_t byte;
	int bytePos;
	uint8_t packet[10];
	int retry;
	int preambleStart;

	/* Sample demod */
	int lastBit;
	uint64_t sampleAt;
	uint64_t sampleNum;
	int samplesPerSymbol;
	runningAvgContext midPointCtx;
	runningAvgContext bitAvgCtx;
} DemodContext;

void demodBit(DemodContext* demodCtx, int bit);
void demodSample(DemodContext* demodCtx, double magnitude);
void demodInit(DemodContext* demodCtx, int samplesPerSymbol, int minPreambleBits, int maxPreambleTimingError, void (*packetHandler)(unsigned char* packet));

#endif // DEMODULATOR_H

