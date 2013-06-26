/****************************************************************************

 Copyright 2003 Matthew Riek
 See legal notice in copyright.txt

 Description : micro xml utf8 support

 ****************************************************************************/

#ifndef _MXMLUTF8_H_
#define _MXMLUTF8_H_

#include <stddef.h>

typedef unsigned char UTF8;

#define MXML_MAX_UTF8_ENCODING 0x0010FFFF

/**
 * \addtogroup MXMLUTF8 Micro XML UTF8
 * \ingroup MXML
 *
 * Micro XML supports parsing of UTF8 encoded XML files.  
 */

extern const int MXMLUtf8ByteCount[256];

/*!
  \ingroup MXMLUTF8
  \brief Convert a UTF32 character (21 bits) to a UTF8 byte sequence
  \param a_inChar is a 21 bit character
  \param a_outBytes must be at least 5 characters, and is filled with the resulting UTF8 sequence
  \return number of bytes written to a_outBytes
*/
int MXMLUtf8FromChar(int a_inChar, char * a_outBytes);

/*!
  \ingroup MXMLUTF8
  \brief Convert a UTF8 byte sequence to a UTF32 (21 bits) character
  \param a_inBytes is a UTF8 byte sequence
  \param a_outChar is set to the UTF32 character
  \return number of bytes read from a_inBytes
*/
int MXMLUtf8ToChar(const char * a_inBytes, int * a_outChar);

/*!
  \ingroup MXMLUTF8
  \brief Get the byte size of a UTF8 byte sequence, excluding the null terminating character
  \param a_string is a UTF8 byte sequence
  \return size in bytes of a_string excluding terminating char
*/
size_t MXMLUtf8Size(const char * a_string);

/*!
  \ingroup MXMLUTF8
  \brief Get the charcter length of a UTF8 byte sequence, excluding the null terminating character
  \param a_string is a UTF8 byte sequence
  \return number of characters excluding null terminating character
*/
size_t MXMLUtf8Length(const char * a_string);

unsigned int MXMLUtf8Hash(const char * a_string);

#endif
