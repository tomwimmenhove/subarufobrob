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


int main(int argc, char** argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "usage, %s <hex string code>\n", argv[0]);
		return -1;
	}

	char* hexString = argv[1];

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

	/* Padding */
	printf("0000000000");

	/* preamble */
	printf("1010101010101010101010101010101010101010101010101010101010101010"
			"1010101010101010101010101010101010101010101010101010101010101010"
			"1010101010101010101010101010101010101010101010101010101010101010"
			"1010101010101010101010101010101010101010101010101010101010101010"
			"10000");
	
	/* Code */
	for (int i= 0; i < 160; i++) printf((encoded[i / 8] & ( 1 << ( 7 - (i % 8)))) ? "1" : "0");

	/* Padding */
	printf("00000000");

	/* Code */
	for (int i= 0; i < 160; i++) printf((encoded[i / 8] & ( 1 << (i % 8))) ? "1" : "0");

	/* Padding */
	printf("0000000000");

	printf("\n");

	return 0;
}

