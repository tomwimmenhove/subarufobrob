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
#include <ctype.h>

unsigned char* hexify(unsigned char* dest, unsigned char* data, int byteLen)
{
	unsigned char* p = dest;

	static char digits[16] = "0123456789ABCDEF";
	while (byteLen--)
	{
		*dest++ = digits[(*data) >> 4];
		*dest++ = digits[*data & 0xf];
		data++;
	}

	*dest = 0;

	return p;
}

static unsigned char hexDigitToNibble(unsigned char digit)
{
	if (isdigit(digit)) return digit - '0';
	if (digit >= 'a' && digit <= 'f') return digit - 'a' + 10;
	if (digit >= 'A' && digit <= 'F') return digit - 'A' + 10;
	fprintf(stderr, "'%c' is not a hex digit!\n", digit);
	return 0;
}

unsigned char* dehexify(unsigned char* dest, unsigned char* hexData, int byteLen)
{
	unsigned char* p = dest;

	while (byteLen--)
	{
		*dest++ = (hexDigitToNibble(hexData[0]) << 4) | hexDigitToNibble(hexData[1]);
		hexData += 2;
	}

	return p;
}

