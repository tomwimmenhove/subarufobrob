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

