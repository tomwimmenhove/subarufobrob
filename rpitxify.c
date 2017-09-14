// This is a modified piam.c from the rpitx package.

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "hex.h"
#include "protocol.h"
#include "manchester.h"

void writeAm(int fd, double a,uint32_t Timing)
{
	typedef struct {
		double a;
		uint32_t WaitForThisSample;
	} rfSample;

	rfSample RfSample;

	RfSample.a=a;
	RfSample.WaitForThisSample=Timing; //en 100 de nanosecond
	if (write(fd,&RfSample,sizeof(rfSample)) != sizeof(rfSample)) {
		fprintf(stderr, "Unable to write sample\n");
	}
}


int main(int argc, char** argv)
{
	int nsecsPerBit = 1013210; // Measures. Might not be 100% accurate, but it works
	if (argc < 3)
	{
		fprintf(stderr, "usage, %s <hex string code> <rpitx rfa file>\n", argv[0]);
		return -1;
	}

	char* hexString = argv[1];
	char* outFile = argv[2];

	if (strlen(hexString) != 20)
	{
		fprintf(stderr, "hex string must be 20 digits (10 byte) long\n");
		return -1;
	}

	/* Turn hex string into binary */
	unsigned char decoded[10];
	dehexify(decoded, hexString, 10);

	/* Manchester encode it */
	unsigned char encoded[20];
	manchester_encode(encoded, decoded, 10);
	
	int fd = open(outFile, O_WRONLY|O_CREAT, 0644);

	/* test padding */
	writeAm(fd, 0, 5000000);
	writeAm(fd, 1, 5000000);
	writeAm(fd, 0, 5000000);

	/* preamble */
	for (int i = 0; i < 128; i++)
	{
		writeAm(fd, 0, nsecsPerBit);
		writeAm(fd, 65535, nsecsPerBit);
	}
	writeAm(fd, 0, 4 * nsecsPerBit);

	printf("Bit code: ");
	/* Code */
	for (int i= 0; i < 160; i++) 
	{
		int bit = encoded[i / 8] & ( 1 << ( 7 - (i % 8)));
		writeAm(fd, bit ? 65535 : 0, nsecsPerBit);
		printf("%d", bit ? 1 : 0);
	}
	printf("\n");

	/* Padding */
	writeAm(fd, 0, 8 * nsecsPerBit);

	/* Code */
	for (int i= 0; i < 160; i++) 
	{
		int bit = encoded[i / 8] & ( 1 << ( 7 - (i % 8)));
		writeAm(fd, bit ? 65535 : 0, nsecsPerBit);
	}

	close(fd);

	return 0;
}

