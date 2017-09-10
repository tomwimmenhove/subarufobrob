#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define COMMAND_LOCK	0x1
#define COMMAND_UNLOCK	0x2
#define COMMAND_TRUNK	0xb

unsigned char getCrc(unsigned char *packet)
{
	return packet[9] & 0xf;
}

void setCrc(unsigned char *packet, unsigned char crc)
{
	packet[9] &= 0xf0;
	packet[9] |= crc;
}

unsigned char calcCrc(unsigned char *packet)
{
	unsigned char crc = 0;
	for (int i = 0; i < 10 - 1; i++)
	{
		crc ^= packet[i] & 0xf;
		crc ^= (packet[i]  >> 4)& 0xf;
	}

	crc ^= (packet[9]  >> 4) & 0xf;
	crc += 1;
	crc &= 0xf;

	return crc;
}

int isValidPacket(unsigned char *packet)
{
	if (packet[0] != 0x55) return -1;
	if (calcCrc(packet) != getCrc(packet)) return -1;
	return 0;
}


unsigned char getCommand(unsigned char *packet)
{
	int commanda = (packet[5] & 0xf);
	int commandb = (packet[6] & 0xf);

	if (commanda != commandb)
	{
		fprintf(stderr, "Commands a and b don't match!\n");
	}

	return commanda;
}

char* commandName(unsigned char command)
{
	switch (command)
	{
		case COMMAND_LOCK:
			return "lock";
		case COMMAND_UNLOCK:
			return "unlock";
		case COMMAND_TRUNK:
			return "trunk";
		default:
			return "unknown";
	}
}

void setCommand(unsigned char *packet, unsigned char command)
{
	packet[5] &= 0xf0;
	packet[5] |= command;
	packet[6] &= 0xf0;
	packet[6] |= command;
}


unsigned int getCode(unsigned char *packet)
{
	return (packet[7] << 12) |
		(packet[8] << 4) |
		(packet[9] >> 4);
}

void setCode(unsigned char *packet, unsigned int code)
{
	packet[7] = (code >> 12) & 0xff;
	packet[8] = (code >>  4) & 0xff;
	packet[9] &= 0xf;;
	packet[9] |= ((code) & 0xf) << 4;
}

