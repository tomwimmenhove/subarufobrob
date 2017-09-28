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
#include <string.h>

char* manchester_decode(char* dest, char*src, int len)
{
	int bitnum = 0;
	int pos = 0;
	unsigned char tmp = 0;

	for (int j= 0; j < len; j++)
	{
		unsigned char byte = src[j];

		for (int i= 0; i < 8; i+=2)
		{
			unsigned a = byte >> 6;

			tmp <<= 1;
			switch (a & 3)
			{
				case 0b10:
					tmp |= 1;
					break;
				case 0b01:
					break;
				default:
					fprintf(stderr, "Error in manchester code\n");
					break;
			}

			bitnum++;

			if (bitnum == 8)
			{
				dest[pos++] = tmp;
				bitnum = 0;
				tmp = 0;
			}

			byte <<= 2;
		}
	}

	return dest;
}

char *manchester_encode(char* dest, char*src, int len)
{
	int bitnum = 0;
	int pos = 0;
	unsigned char tmp = 0;

	for (int j = 0; j < len; j++)
	{
		unsigned char byte = src[j];

		for (int i= 0; i < 8; i++)
		{
			tmp <<= 2;

			if (byte & 0x80)
			{
				tmp |= 0x2;
			}
			else
			{
				tmp |= 0x1;
			}

			bitnum += 2;

			if (bitnum == 8)
			{
				dest[pos++] = tmp;
				bitnum = 0;
				tmp = 0;
			}

			byte <<= 1;
		}
	}

	return dest;
}

