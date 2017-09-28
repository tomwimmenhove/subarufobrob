/*
 *  Copyright 2017 Tom Wimmenhove<tom.wimmenhove@gmail.com>
 *
 *  This file is part of Subarufobrob
 *
 *  Subarufobrob is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Subarufobrob is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Subarufobrob.  If not, see <http://www.gnu.org/licenses/>.
 */

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

