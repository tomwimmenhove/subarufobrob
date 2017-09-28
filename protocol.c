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

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "protocol.h"

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
		case COMMAND_PANIC:
			return "panic";
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

