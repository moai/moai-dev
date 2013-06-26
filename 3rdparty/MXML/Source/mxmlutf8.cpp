/****************************************************************************

 Copyright 2003 Matthew Riek
 See legal notice in copyright.txt

 Description : micro xml utf8 support

 ****************************************************************************/

#include "mxmlutf8.h"
#include <string.h>


static const int MXMLutf8FirstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

const int MXMLUtf8ByteCount[256] = 
{
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,1,1,1,1,1,1,1,1,1,1,1,
};


int MXMLUtf8FromChar(int a_inChar, char * a_outBytes)
{
  int outNumBytes = 0;
  UTF8 * outBytes;

  /* Determine output byte count */
  if(a_inChar < 0x00000080) { *a_outBytes = (char) a_inChar; return 1; } /* optimal common case */
  else if(a_inChar < 0x00000800) { outNumBytes = 2; } 
  else if(a_inChar < 0x00010000) { outNumBytes = 3; } 
  else if(a_inChar <= MXML_MAX_UTF8_ENCODING) { outNumBytes = 4; } 
  else { outNumBytes = 3; a_inChar = 0x0000FFFD; } /* replacement char */

  outBytes = (UTF8 *) (a_outBytes + outNumBytes);
  switch(outNumBytes)
  {
    case 4: *--outBytes = (char)((a_inChar | 0x80) & 0xBF); a_inChar >>= 6;
    case 3: *--outBytes = (char)((a_inChar | 0x80) & 0xBF); a_inChar >>= 6;
    case 2: *--outBytes = (char)((a_inChar | 0x80) & 0xBF); a_inChar >>= 6;
    case 1: *--outBytes = (char) (a_inChar | MXMLutf8FirstByteMark[ outNumBytes ]);
  }
  return outNumBytes;
}


int MXMLUtf8ToChar(const char * a_inBytes, int * a_outChar)
{
  const UTF8 * inBytes = (const UTF8 *) a_inBytes;
  switch (MXMLUtf8ByteCount[*inBytes])
  {
    case 1:
      *a_outChar = (int) inBytes[0];
      return 1;
    case 2:
      *a_outChar = (int) (((inBytes[0] & 0x1F) << 6) | (inBytes[1] & 0x3F));
      return 2;
    case 3:
      *a_outChar = (int) (((inBytes[0] & 0x0F) << 12) | ((inBytes[1] & 0x3F) << 6) | (inBytes[2] & 0x3F));
      return 3;
    case 4:
      *a_outChar = (int) (((inBytes[0] & 0x07) << 18) | ((inBytes[1] & 0x3F) << 12) | ((inBytes[2] & 0x3F) << 6) | (inBytes[3] & 0x3F));
      return 4;
    default:
      break;
  }
  return 1;
}


size_t MXMLUtf8Size(const char * a_string)
{
#if 1
  return strlen(a_string);
#else
  register const UTF8 * string = (const UTF8 *) a_string;
  size_t size = 0;
  while(*string)
  {
    size_t numBytes = (size_t) MXMLUtf8ByteCount[*string];
    string += numBytes;
    size += numBytes;
  }
  return size;
#endif
}


size_t MXMLUtf8Length(const char * a_string)
{
  register const UTF8 * string = (const UTF8 *) a_string;
  size_t length = 0;
  while(*string)
  {
    size_t numBytes = (size_t) MXMLUtf8ByteCount[*string];
    string += numBytes;
    length += 1;
  }
  return length;
}


unsigned int MXMLUtf8Hash(const char * a_string)
{
  unsigned int i;
  for(i = 0; *a_string; a_string++) 
  {
    char c = *a_string;
    c = (char) (c - (c & (c >> 1) & 0x20));
    i = 131 * i + c;
  }
  return i;
}
