#include "main.h"

//#include <string.h>should be inside bzero
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

void stringtohex(const TCHAR* text, TCHAR bytes[], size_t byteCount)
{
  unsigned int temp;

  for (size_t i = 0; i < byteCount; ++i)
  {
    int result;
#ifdef UNICODE
    result = swscanf(text + 2 * i, _T("%2x"), &temp); // Parse two hexadecimal characters
#else
    result = sscanf(text + 2 * i, "%2x", &temp);
#endif

    if (result != 1) // Check if exactly one item was parsed successfully
    {
      bytes[i] = _T('\0'); // Null terminate to indicate failure
      _tprintf(_T("Error: Unable to parse hex value at position %zu\n"), i);
      return; // Exit on failure
    }

    bytes[i] = (TCHAR) temp; // Store the value as a TCHAR
  }

  bytes[byteCount] = _T('\0'); // Null-terminate the TCHAR array
}

/*
 *  Convert a HEXADECIMAL character to a byte.
 *  Returns 255 if an invalid character provided.
 */

int char2hex(char c)
{
  if (c >= '0' && c <='9')
    return ( c - 48);
  if ( (c >= 'a') && (c <= 'f') )
    return (c-'a' + 10);
  
  if ( (c >= 'A') && (c <='F') )
    return (c-'A' + 10);
  
  return -1;
}

int hex2char(char c)
{
  if (c <=9)
    return (c+48);

  return (c - 10 + 'a');
}




byte hex2byte(const char c) {
  if (c >= '0' && c <='9') {
    return c - '0' + 0;
  }
  if (c >= 'A' && c <= 'F') {
    return c - 'A' + 10;
  }
  return 255;
}

/*
 *  Convert a byte data to a HEXADECIMAL character.
 *  Returns '*' if an invalid byte data which is larger than
 *  15 provided.
 */
char byte2hex(const byte d) {
  if (d <= 9) {
    return '0' + (d - 0);
  }
  if (d <= 15) {
    return 'A' + (d - 10);
  }
  return '*';
}

/*
 * convert number like "1234" to 0x32, 0x54
 */
#define CONV2PIN(x) (((x) - '0') + 1)
void num2pin(char *num, u_char *pin)
{
	int i, j;
	bzero(pin, 8);
	for(i = j = 0; i < 16 && num[i] != '\0'; i++) {
		if(i & 1)
			pin[j++] |= CONV2PIN(num[i]) << 4;
		else
			pin[j] = CONV2PIN(num[i]);
	}
}

/*
 * convert pin number like 0x32, 0x54 to "1234"
 */
#define CONV2NUM(x) ((x) > 0 ? (((x) - 1) + '0') : '\0')
void pin2num(u_char *pin, char *num)
{
	int i, j;
	bzero(num, 16);
	for(i = j = 0; i < 8; i++) {
		num[j++] = CONV2NUM(pin[i] & 0xf);
		num[j++] = CONV2NUM((pin[i] >> 4) & 0xf);
	}
}
