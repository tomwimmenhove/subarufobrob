#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char** argv)
{
	char* line = argv[1];

	double samplesPerBit = 194.536313;
	double bitsPerSample = 1.0 / samplesPerBit;
	double bits = 0.0;

	int len = strlen(line);

	float out[len * ((int) samplesPerBit + 1) + 1];
	float preamble[512];

	int nBits = 0;

	fprintf(stderr, "fbitstring: %s\n", line);

	for (int i = 0; i < len;i ++)
	{
		int bit = ( (line[i] == '1') ? 1 : 0);

		while (bits < i)
		{
//			out[nBits++] = bit ? f : 0.0;
			out[nBits++] = bit ? 0.0 : -1.0;
			bits += bitsPerSample;
		}
	}

	if (write(1, out, nBits * sizeof(float)) == -1)
	{
		perror("write");
	}

	return 0;
}

