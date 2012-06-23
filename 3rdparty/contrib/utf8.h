/*
  Basic UTF-8 manipulation routines
  by Jeff Bezanson
  placed in the public domain Fall 2005

  This code is designed to provide the utilities you need to manipulate
  UTF-8 as an internal string encoding. These functions do not perform the
  error checking normally needed when handling UTF-8 data, so if you happen
  to be from the Unicode Consortium you will want to flay me alive.
  I do this because error checking can be performed at the boundaries (I/O),
  with these routines reserved for higher performance on data known to be
  valid.
*/

/* http://www.cprogramming.com/tutorial/unicode.html */

/* with some tweaks by Patrick Meehan */

#ifndef CONTRIB_UTF8_H
#define CONTRIB_UTF8_H

#include <stdarg.h>

#ifdef __cplusplus
	extern "C" {
#endif

#ifdef _WIN32
		typedef unsigned long u_int32_t;
#else
		#define _snprintf snprintf
#endif

#ifdef NACL
		typedef unsigned long u_int32_t;
#endif

/* is c the start of a utf8 sequence? */
#define isutf(c) (((c)&0xC0)!=0x80)

/* convert UTF-8 data to wide character */
extern int u8_toucs(u_int32_t *dest, int sz, char *src, int srcsz);

/* the opposite conversion */
extern int u8_toutf8(char *dest, int sz, u_int32_t *src, int srcsz);

/* single character to UTF-8 */
extern int u8_wc_toutf8(char *dest, u_int32_t ch);

/* character number to byte offset */
extern int u8_offset(char *str, int charnum);

/* byte offset to character number */
extern int u8_charnum(char *s, int offset);

/* return next character, updating an index variable */
extern u_int32_t u8_nextchar(const char *s, int *i);

/* move to next character */
extern void u8_inc(char *s, int *i);

/* move to previous character */
extern void u8_dec(char *s, int *i);

/* returns length of next utf-8 sequence */
extern int u8_seqlen(char *s);

/* assuming src points to the character after a backslash, read an
   escape sequence, storing the result in dest and returning the number of
   input characters processed */
extern int u8_read_escape_sequence(char *src, u_int32_t *dest);

/* given a wide character, convert it to an ASCII escape sequence stored in
   buf, where buf is "sz" bytes. returns the number of characters output. */
extern int u8_escape_wchar(char *buf, int sz, u_int32_t ch);

/* convert a string "src" containing escape sequences to UTF-8 */
extern int u8_unescape(char *buf, int sz, char *src);

/* convert UTF-8 "src" to ASCII with escape sequences.
   if escape_quotes is nonzero, quote characters will be preceded by
   backslashes as well. */
extern int u8_escape(char *buf, int sz, char *src, int escape_quotes);

/* utility predicates used by the above */
extern int octal_digit(char c);
extern int hex_digit(char c);

/* return a pointer to the first occurrence of ch in s, or NULL if not
   found. character index of found character returned in *charn. */
extern char *u8_strchr(char *s, u_int32_t ch, int *charn);

/* same as the above, but searches a buffer of a given size instead of
   a NUL-terminated string. */
extern char *u8_memchr(char *s, u_int32_t ch, size_t sz, int *charn);

/* count the number of characters in a UTF-8 string */
extern int u8_strlen(const char *s);

extern int u8_is_locale_utf8(char *locale);

/* printf where the format string and arguments may be in UTF-8.
   you can avoid this function and just use ordinary printf() if the current
   locale is UTF-8. */
extern int u8_vprintf(char *fmt, va_list ap);
extern int u8_printf(char *fmt, ...);

#ifdef __cplusplus
	}
#endif

#endif