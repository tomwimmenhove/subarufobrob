#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "hex.h"
#include "protocol.h"

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "usage, %s <command> [increment]\n\tcommand can be \"lock\", \"unlock\" or \"trunk\"\nincrement increments the rolling code by a certain amound. Defaults to 1\n", argv[0]);
		return -1;
	}

	int inc = 1;
	if (argc > 2)
	{
		inc = atoi(argv[2]);
	}


	int fd = open("latestcode.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("Could not open latestcode.txt");
		return -1;
	}

	char hexString[21];
	if ((read(fd, hexString, 20)) < 20)
	{
		fprintf(stderr, "Couldn't read a full hex string from latestcode.txt\n");
		return -1;
	}
	hexString[20] = 0; // terminator

	int forceCommand;

	if (strcmp(argv[1], "lock") == 0) forceCommand = COMMAND_LOCK;
	else if (strcmp(argv[1], "unlock") == 0) forceCommand = COMMAND_UNLOCK;
	else if (strcmp(argv[1], "trunk") == 0) forceCommand = COMMAND_TRUNK;
	else
	{
		fprintf(stderr, "Dunno what %s means, use \"lock\", \"unlock\" or \"trunk\"\n", argv[2]);
		return -1;
	}

	int n = 0;

	if (strlen(hexString) != 20)
	{
		fprintf(stderr, "hex string must be 20 digits (10 byte) long\n");
		return -1;
	}

	fprintf(stderr, "Current code: %s\n", hexString);

	unsigned char decoded[10];

	dehexify(decoded, hexString, 10);

	unsigned char mycrc = calcCrc(decoded);;
	unsigned char crc = getCrc(decoded);

	int rollingCode = getCode(decoded);

	/* next rolling code */
	setCode(decoded, rollingCode + inc);

	/* Force command */
	setCommand(decoded, forceCommand);

	/* Fix CRC */
	setCrc(decoded, calcCrc(decoded));

	/* Output the new code */
	unsigned char newHexString[21];
	hexify(newHexString, decoded, 10);

	fprintf(stderr, "New code    : %s\n", newHexString);

	/* Save to file */
	FILE* f = fopen("latestcode.txt", "w");
	fprintf(f, "%s\n", newHexString);
	fclose(f);

	return 0;
}

