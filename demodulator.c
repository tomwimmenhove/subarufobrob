#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "protocol.h"
#include "runningavg.h"

#include "demodulator.h"

#define DEMOD_PREAMBLE		0
#define DEMOD_WAITZEROES	1
#define DEMOD_BITBANG		2
#define DEMOD_WAITRETRY		3

void demodBit(DemodContext* demodCtx, int bit)
{
	switch (demodCtx->state)
	{
		case DEMOD_PREAMBLE: // Wait for preamble
			/* Check if timing is somewhere enar reasonable */
			if (demodCtx->preambleGood)
			{
				/* We check the times between ones */
				if (bit)
				{
					if (demodCtx->lastOneSampledAt)
					{
						/* Reset state if there's too large a discrepancy */
						if (abs(demodCtx->sampleNum - demodCtx->lastOneSampledAt - demodCtx->samplesPerSymbol * 2) > demodCtx->maxPreambleTimingError)
						{
							demodCtx->preambleGood = 0;
							demodCtx->lastOneSampledAt = 0;
							break;
						}
					}
					demodCtx->lastOneSampledAt = demodCtx->sampleNum;
				}
			}

			/* Check for a series of alternating bits */
			if (demodCtx->lastDemodBit != bit)
			{
				if (demodCtx->preambleGood == 1)
				{
					demodCtx->preambleStart = demodCtx->sampleNum; 
				}
				demodCtx->preambleGood++;
			}
			else
			{
				demodCtx->preambleGood = 0;
				demodCtx->lastOneSampledAt = 0;
			}

			/* Do we have enough of a preamble to be confident that it's the start of a packet ? */
			if (demodCtx->preambleGood >= demodCtx->minPreamble)
			{
//				fprintf(stderr, "Symbols per sample for this preamble: %f\n", (double) (demodCtx->sampleNum - demodCtx->preambleStart) / (demodCtx->minPreamble - 2));
				demodCtx->state = DEMOD_WAITZEROES;
				demodCtx->preambleGood = 0;
				demodCtx->lastOneSampledAt = 0;
				demodCtx->ending = 0;
			}
			break;
		case DEMOD_WAITZEROES: // wait for en of preamble: 4 zeroes in a row
			if (bit == 0)
			{
				demodCtx->ending++;
				/* Get ready for the next state if we have our 4 zeroes */
				if (demodCtx->ending == 4)
				{
					fprintf(stderr, "Got preamble!\n");
					demodCtx->ending = 0;
					demodCtx->manchPos = 0;
					demodCtx->byte = 0;
					demodCtx->bitpos = 0;
					demodCtx->bytePos = 0;
					demodCtx->retry = 1;
					demodCtx->state = DEMOD_BITBANG;
				}
			}
			else
			{
				demodCtx->ending = 0;
				if (demodCtx->ending > 1)
				{
					/* We just had more than 1 zero followed by a one: not a valid preamble: reset state */
					demodCtx->ending = 0;
					demodCtx->state = DEMOD_PREAMBLE;
				}
			}

			break;
		case DEMOD_BITBANG: // bit bang
			if (demodCtx->manchPos & 1)
			{       
				/* Check for manchester encoding errors */
				if (bit == demodCtx->lastDemodBit)
				{
//					fprintf(stderr, "WARNING: Manchester encoding error: %d and %d at manchPos %d\n", bit, demodCtx->lastDemodBit, demodCtx->manchPos);

					if (!demodCtx->retry)
					{
						/* Reset state */
						demodCtx->state = DEMOD_PREAMBLE;
						demodCtx->preambleGood = 0;
						demodCtx->lastOneSampledAt = 0;
					}
				}
			}
			else
			{
				/* This is a data bit */
				demodCtx->byte <<= 1;
				demodCtx->byte |= bit;

				demodCtx->bitpos++;

				/* Do we have a full byte? */
				if (demodCtx->bitpos == 8)
				{       
					/* Add to buffer */
					demodCtx->packet[demodCtx->bytePos++] = demodCtx->byte;
					demodCtx->byte = 0;
					demodCtx->bitpos = 0;

					/* Do we have a full packet? */
					if (demodCtx->bytePos == sizeof(demodCtx->packet))
					{
						if (isValidPacket(demodCtx->packet) == -1)
						{
							fprintf(stderr, "Invalid packet\n");
							if (demodCtx->retry == 0)
							{
								/* No more retries: reset state */
								demodCtx->state = DEMOD_PREAMBLE;
							}
							else
							{
								/* Try the next transmission? */
								demodCtx->state = DEMOD_WAITRETRY;
							}
						}
						else
						{
							demodCtx->packetHandler(demodCtx->packet);
							demodCtx->preambleGood = 0;
							demodCtx->lastOneSampledAt = 0;
							demodCtx->state = DEMOD_PREAMBLE;
						}

						demodCtx->retry--;
					}
				}
			}

			demodCtx->manchPos++;
			break;

		case DEMOD_WAITRETRY: // Wait 5 bits for the next iransmission
			demodCtx->manchPos++;
			if (demodCtx->manchPos == 164)
			{
				demodCtx->manchPos = 0;
				demodCtx->byte = 0;
				demodCtx->bitpos = 0;
				demodCtx->bytePos = 0;

				demodCtx->state = DEMOD_BITBANG;
			}
			break;

	}

	demodCtx->lastDemodBit = bit;
}

void demodSample(DemodContext* demodCtx, double magnitude)
{
	/* And get a running average of one bitlength of that */
	double avgSample = runningAvg(&demodCtx->bitAvgCtx, magnitude);

	/* Get a mid point to compare levels against */
	double midPoint = runningAvg(&demodCtx->midPointCtx, avgSample);

	/* Get the value of the actual bit */
	int bit = avgSample > midPoint;

	/* Check for zero-crossing */
	if (demodCtx->lastBit != bit)
	{
		// Align the sampleAt variable here
		demodCtx->sampleAt = demodCtx->sampleNum + demodCtx->samplesPerSymbol / 2.0;
	}

	/* Is this the middle of our sample ? */
	if (demodCtx->sampleNum >= demodCtx->sampleAt)
	{
		demodBit(demodCtx, bit);

		/* Time the next sample */
		demodCtx->sampleAt += demodCtx->samplesPerSymbol;
	}

	demodCtx->lastBit = bit;
	demodCtx->sampleNum++;
}

void demodInit(DemodContext* demodCtx, int samplesPerSymbol, int minPreambleBits, int maxPreambleTimingError, void (*packetHandler)(unsigned char* packet))
{
	memset(demodCtx, 0, sizeof(DemodContext));
	demodCtx->minPreamble = minPreambleBits;
	demodCtx->maxPreambleTimingError = maxPreambleTimingError;
	demodCtx->samplesPerSymbol = samplesPerSymbol;
	runningAvgInit(&demodCtx->bitAvgCtx, samplesPerSymbol);
	runningAvgInit(&demodCtx->midPointCtx, samplesPerSymbol * 8); // XXX: 8 is a guess. Significantly longer than any period without a zero-crossing, shorter than the preamble
	demodCtx->packetHandler = packetHandler;
}

