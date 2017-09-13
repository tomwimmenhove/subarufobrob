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

#include "hex.h"
#include "protocol.h"

void handlePacket(unsigned char* packet)
{
	if (isValidPacket(packet) == -1)
	{
		return;
	}

	unsigned char hexString[21];

	hexify(hexString, packet, 10);

	fprintf(stderr, "Valid packet received\n");
	fprintf(stderr, " * Code: %s\n", hexString);
	fprintf(stderr, " * Command: %s\n", commandName(getCommand(packet)));
	fprintf(stderr, " * Rolling code: %d\n", getCode(packet));

	FILE* f = fopen("latestcode.txt", "w");
	fprintf(f, "%s\n", hexString);
	fclose(f);

	f = fopen("receivedcodes.txt", "a");
	fprintf(f, "%s\n", hexString);
	fclose(f);
}

double runningPeak(double sample, double *history, int historyPos, int len, double currentPeak)
{
	historyPos %= len;
	history[historyPos] = sample;

	if (sample > currentPeak) return sample;

	/* if the possibel peak 'slid' off */
	int nextPos = (historyPos + 1) % len;
	if (history[nextPos] == currentPeak)
	{
		history[nextPos] = 0;

		double peak = 0.0;
		/* Recalculate the peak */
		for (int i=0; i < len - 1; i++)
		{
			if (history[i] > peak) peak = history[i];
		}

		return peak;
	}
	return currentPeak;
}

int main(int argc, char **argv)
{
        float buf[192]; // 1ms
        double samplesPerBit = 194.536313 / 2.0;
	double preambleMaxError = samplesPerBit / 10.0;
	double sampleAt = 0.0;
	int state = 0;
	int lastCrossing = 0;
	int manchPos = 0;
	int bitpos = 0;
	int n;
	int sampleNum = 0;
	int lastBit = 0;
	int preambleGood = 0;
	int minPreambleBits = 42;
	unsigned char byte;
	unsigned char packet[10];
	int pos = 0;
	int lastSampledBit;

	int historyLen = 1024; // XXX: Depend on samplerate
	double history[historyLen];
	int historyPos = 0;

	double peak = 0;

	int decimation = 25;
	int decimator = 0;

	memset(history, 0, sizeof(history));

	while ((n = read(0, buf, sizeof(buf))) > 0)
	{
		for (int i = 0 ; i < n / sizeof(float); i++)
		{
			float sample = buf[i];

			if (decimation > ++decimator)
			{
				continue;
			}
			decimator = 0;

			peak = runningPeak(sample, history, sampleNum, historyLen, peak);

			/* 4, because magnitude^2 */
			int bit = sample > peak / 4;

			/* Check for zero-crossing */
			int zeroCrossing = (lastBit != bit);
			int sinceLastCrossing = sampleNum - lastCrossing;

			if (zeroCrossing)
			{
				// Align the sampleAt variable here
				sampleAt = sampleNum + samplesPerBit / 2.0;
			}

			/* Check if this is the end of the preamble */
			if (preambleGood >= minPreambleBits)
			{
				/* Pre-amble is followed by 4 low 'bits' */
				if (fabs(sinceLastCrossing / 4 - samplesPerBit) < preambleMaxError)
				{
					state = 1;
					preambleGood = 0;

					/* Start sampling halfway during the next bit */
					sampleAt = sampleNum + samplesPerBit / 2.0;
					break;
				}
			}

			switch(state)
			{
				case 0: // wait for pre-amble
					if (zeroCrossing)
					{ 
						if (!preambleGood) /* See if this is the start of a preamble */
						{
							preambleGood++;
							if (preambleGood > 5)
							{
								fprintf(stderr, "God damit, wtf: %d\n", preambleGood);
							}
						}
						else
						{
							float err = fabsf((double) sinceLastCrossing - samplesPerBit);;
							if (err > preambleMaxError)
							{
								preambleGood = 0;
							}
							else
							{
								preambleGood++;
							}
						}
					}
					break;
				case 1: // prepare for bitbang
//					fprintf(stderr, "Got a packet!\n");
					manchPos = 0;
					state = 2;
					bitpos = 0;
					pos = 0;
					byte = 0;
					// fall through
				case 2: // bitbang
					if (sampleNum >= sampleAt)
					{
						/* Time the next sample */
						sampleAt += samplesPerBit;

						if (manchPos & 1)
						{
							/* Check for manchester encoding errors */
							if (bit == lastSampledBit)
							{
//								fprintf(stderr, "Manchester encoding error: %d and %d at manchPos %d\n", bit, lastSampledBit, manchPos);
								/* Reset state */
								state = 0;
							}
						}
						else
						{
							/* This is a data bit */
							byte <<= 1;
							byte |= bit;

							bitpos++;

							if (bitpos == 8)
							{
								packet[pos++] = byte;
								byte = 0;
								bitpos = 0;

								if (pos == sizeof(packet)) // We have a full packet! */
								{
									handlePacket(packet);
									state = 0;
								}
							}
						}

						manchPos++;
						lastSampledBit = bit;
					}

					break;
			}

			if (zeroCrossing)
			{
				lastCrossing = sampleNum;
			}

			lastBit = bit;
			sampleNum++;
		}
	}

	return 0;
}
